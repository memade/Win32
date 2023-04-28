#include "stdafx.h"

namespace local {

 Server::Server() {
  Init();
 }
 Server::~Server() {
  UnInit();
 }
 void Server::Release() const {
  delete this;
 }
 void Server::Init() {
  m_pLog = new shared::Log();
  uv_loop_t_ = new uv_loop_t;
  uv_connect_t_ = new uv_connect_t;
  uv_thread_main_ = new uv_thread_t;
  uv_async_t_ = new uv_async_t;
  m_pConfig = new Config();
 }
 void Server::UnInit() {
  SK_DELETE_PTR(uv_loop_t_);
  SK_DELETE_PTR(uv_server_);
  SK_DELETE_PTR(uv_connect_t_);
  SK_DELETE_PTR(uv_async_t_);
  SK_DELETE_PTR(uv_thread_main_);
  SK_DELETE_PTR(m_pConfig);
  SK_DELETE_PTR(m_pLog);
 }
 IConfig* Server::ConfigGet() const {
  return dynamic_cast<IConfig*>(m_pConfig);
 }
 bool Server::Start() {
  int r = -1;
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   m_Status.store(ServerStatus::Opening);
   if (r = uv_loop_init(uv_loop_t_) != 0)
    break;
   if (r = uv_async_init(uv_loop_t_, uv_async_t_, AsyncCb) != 0)
    break;
   switch (m_pConfig->ServerType()) {
   case EnServerType::TCP: {
    uv_server_ = reinterpret_cast<uv_stream_t*>(new uv_tcp_t);
    struct sockaddr_in addr = { 0 };
    if (r = uv_ip4_addr(m_pConfig->IPAddrV4().c_str(), m_pConfig->Port(), &addr) != 0)
     break;
    if (r = uv_tcp_init(uv_loop_t_, (uv_tcp_t*)uv_server_) != 0)
     break;
    if (r = uv_tcp_bind((uv_tcp_t*)uv_server_, (const struct sockaddr*)&addr, 0) != 0)
     break;
    uv_server_->data = this;
    if (r = uv_listen(uv_server_, SOMAXCONN, ConnectionCb) != 0)
     break;
   }break;
   case EnServerType::UDP: {
    uv_server_ = reinterpret_cast<uv_stream_t*>(new uv_udp_t);
    struct sockaddr_in addr = { 0 };
    if (r = uv_ip4_addr(m_pConfig->IPAddrV4().c_str(), m_pConfig->Port(), &addr) != 0)
     break;
    if (r = uv_udp_init(uv_loop_t_, (uv_udp_t*)uv_server_) != 0)
     break;
    if (r = uv_udp_bind((uv_udp_t*)uv_server_, (const struct sockaddr*)&addr, 0) != 0)
     break;
    if (r = uv_udp_recv_start((uv_udp_t*)uv_server_, AllocCb, UdpRecvCb) != 0)
     break;
   }break;
   case EnServerType::PIPE: {
    uv_server_ = reinterpret_cast<uv_stream_t*>(new uv_pipe_t);
    if (r = uv_pipe_init(uv_loop_t_, (uv_pipe_t*)uv_server_, 0) != 0)
     break;
    if (r = uv_pipe_bind((uv_pipe_t*)uv_server_, m_pConfig->PipeName().c_str()) != 0)
     break;
    if (r = uv_listen(uv_server_, SOMAXCONN, ConnectionCb) != 0)
     break;
   }break;
   default:
    break;
   }


   if (r != 0)
    break;
   m_Threads.emplace_back([this]() {Process(); });
   notify_parent_process();
   if (r = uv_thread_create(uv_thread_main_,
    [](void* arg) {
     uv_run(reinterpret_cast<Server*>(arg)->uv_loop_t_, UV_RUN_DEFAULT);
    }, this) != 0)
    break;
  } while (0);
  m_Status.store(r == 0 ? ServerStatus::Ready : ServerStatus::Unready);
  LOG_OUTPUT(m_pLog, 
   std::format("Server[{}:{}] type({}) {}.",
    m_pConfig->IPAddrV4(),
    m_pConfig->Port(), 
    unsigned long(m_pConfig->ServerType()), 
    m_Status.load() == ServerStatus::Ready ?"success":"failed"));
  return m_IsOpen.load();
 }
 void Server::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_Status.store(ServerStatus::Closing);

   m_Sessions.iterate_clear(
    [&](const u_short& key, Session* pSession, bool& itbreak, bool& itclear) {
     pSession->ForceClose();
    });
   auto async_req = new async_req_t;
   async_req->host = this;
   async_req->req_type = async_req_type::Close;
   uv_async_t_->data = async_req;
   uv_async_send(uv_async_t_);

   m_IsOpen.store(false);
   for (auto& it : m_Threads)
    it.join();
   m_Threads.clear();

   ILibuv::uv_close_loop(uv_loop_t_);

   uv_thread_join(uv_thread_main_);

   m_Sessions.iterate_clear(
    [&](const u_short& key, Session* pSession, bool& itbreak, bool& itclear) {
     pSession->Release();
     itclear = true;
    });

   m_Status.store(ServerStatus::Closed);
  } while (0);
 }
 ServerStatus Server::Status() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load();
 }

 //!@ private
 void Server::OnConnent(Session* pSession) {
  if (pSession && pSession->Ready()) {
   pSession->Write(CommandType::Welcome, "Welcome to server 1.0 .");
   m_Sessions.push(pSession->Key(), pSession);
   LOG_OUTPUT(m_pLog, std::format("session logon on({}:{})", pSession->IP(), pSession->Port()));
  }
  else if (pSession) {
   pSession->Release();
  }
 }
 //!@ private
 void Server::Process() {
  do {
   const std::uint64_t time_current_ms = shared::Win::Time::TimeStamp<std::chrono::milliseconds>();

   m_Sessions.iterate_clear(
    [&](const u_short& key, Session* pSession, bool& itbreak, bool& itclear) {
     const auto session_status = pSession->Status();
     if (session_status == SessionStatus::Closed) {
      LOG_OUTPUT(m_pLog, std::format("session logout on({}:{})", pSession->IP(), pSession->Port()));
      pSession->Release();
      itclear = true;
     }
     else if (pSession->Ready()) {
      bool handle_result = false;
      do {
       if (!pSession->KeepAliveCheck(time_current_ms))
        break;
       if (!pSession->Write())
        break;
       std::string read_data = pSession->Read();
       if (!read_data.empty()) {
        HEAD head;
        std::string message;
        if (!Protocol::UnMakeStream(read_data, head, message))
         break;
        switch (head.Command()) {
        case CommandType::Hello: {
         pSession->Write(CommandType::Welcome, "Welcome to server 1.0 .");
        }break;
        default:
         break;
        }

        LOG_OUTPUT(m_pLog,
         std::format("recv session({}:{}) message({:X}:{}).",
          pSession->IP(),
          pSession->Port(),
          static_cast<unsigned long>(head.Command()),
          message
         ));
        pSession->KeepAliveUpdate(time_current_ms);
       }


       handle_result = true;
      } while (0);
      if (!handle_result)
       pSession->ForceClose();
     }
     else {

     }
    });
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   if (!m_IsOpen.load())
    break;
  } while (1);
 }

 void Server::AsyncCb(uv_async_t* async) {
  async_req_t* req = (async_req_t*)async->data;
  Server* pServer = (Server*)req->host;
  switch (req->req_type) {
  case async_req_type::Close: {
   uv_close((uv_handle_t*)async,
    [](uv_handle_t* handle) {

     auto sk = 0;
    });
   uv_close((uv_handle_t*)pServer->uv_server_,
    [](uv_handle_t* handle) {

     auto sk = 0;
    });
   // ILibuv::uv_close_stream(uv_server_);
  }break;
  default:
   break;
  }
  SK_DELETE_PTR(req);
 }

 void Server::ConnectionCb(uv_stream_t* server, int status) {
  Server* pServer = reinterpret_cast<Server*>(server->data);
  if (pServer->Status() == ServerStatus::Ready && status == 0) {
   Session* pSession = new Session(EnSessionType::TCP_SESSION_SERVER);
   pSession->SetServerStream(server);
   pServer->OnConnent(pSession);
  }
  else {

  }
 }
 void Server::AllocCb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
#if 0
  /* up to 16 datagrams at once */
  static char slab[16 * 64 * 1024];
  buf->base = slab;
  buf->len = sizeof(slab);
#endif
  auto ss = 0;
 }
 void Server::UdpRecvCb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
  auto ss = 0;
#if 0
  uv_buf_t sndbuf;
  uv_udp_send_t* req;

  if (nread == 0) {
   /* Everything OK, but nothing read. */
   return;
  }

  ASSERT(nread > 0);
  ASSERT(addr->sa_family == AF_INET);

  req = send_alloc();
  ASSERT_NOT_NULL(req);
  sndbuf = uv_buf_init(rcvbuf->base, nread);
  ASSERT(0 <= uv_udp_send(req, handle, &sndbuf, 1, addr, on_send));
#endif
 }
 Server* __gpServer = nullptr;
}///namespace local
