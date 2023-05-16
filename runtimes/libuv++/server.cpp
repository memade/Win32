#include "stdafx.h"

namespace local {

 Server::Server(const SessionType& session_type)
  : m_SessionType(session_type) {
  Init();
 }
 Server::~Server() {
  UnInit();
 }
 void Server::Release() const {
  delete this;
 }
 void Server::Init() {
  LOG_INIT;
  m_pConfig = new Config();
  m_pReadStream = new Stream(0xFFFF, StreamType::Read);
  m_pWriteStream = new Stream(0xFFFF, StreamType::Write);
 }
 void Server::UnInit() {
  SK_DELETE_PTR(m_pConfig);
  SK_DELETE_PTR(m_pLoop);
  SK_DELETE_PTR(m_pServer);
  SK_DELETE_PTR(m_pAsync);
  SK_DELETE_PTR(m_pReadStream);
  SK_DELETE_PTR(m_pWriteStream);
  LOG_UNINIT;
 }
 IConfig* Server::ConfigGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return dynamic_cast<IConfig*>(m_pConfig);
 }
 unsigned long Server::SessionCount() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return static_cast<unsigned long>(m_Sessions.size());
 }
 UvLoop* Server::LoopHandle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pLoop;
 }
 UvHandle* Server::AsyncHandle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pAsync;
 }
 UvHandle* Server::ServerHandle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pServer;
 }
 bool Server::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);

   switch (m_SessionType) {
   case SessionType::TCP_SESSION_SERVER: {
    m_pLoop = new UvLoop();
    m_pServer = new UvHandle(HandleType::UV_TCP, m_pLoop, this);
    m_pAsync = new UvHandle(HandleType::UV_ASYNC, m_pLoop, this);
    struct sockaddr_in addr = { 0 };
    if (0 != uv_ip4_addr(m_pConfig->IPAddrV4().c_str(), m_pConfig->Port(), &addr))
     break;
    if (0 != uv_tcp_bind(T_GET_HANDLE<uv_tcp_t>(m_pServer), (const struct sockaddr*)&addr, 0))
     break;
    if (0 != uv_listen(T_GET_HANDLE<uv_stream_t>(m_pServer), SOMAXCONN, Server::OnConnect))
     break;
   }break;
   case SessionType::IPC_SESSION_SERVER: {
    m_pLoop = new UvLoop();
    m_pServer = new UvHandle(HandleType::UV_PIPE_0, m_pLoop, this);
    m_pAsync = new UvHandle(HandleType::UV_ASYNC, m_pLoop, this);
    if (0 != uv_pipe_bind(T_GET_HANDLE<uv_pipe_t>(m_pServer), m_pConfig->PipeName().c_str()))
     break;
    if (0 != uv_listen(T_GET_HANDLE<uv_stream_t>(m_pServer), SOMAXCONN, Server::OnConnect))
     break;
   }break;
   case SessionType::UDP_SESSION_SERVER: {
    m_pLoop = new UvLoop();
    m_pServer = new UvHandle(HandleType::UV_UDP, m_pLoop, this);
    m_pAsync = new UvHandle(HandleType::UV_ASYNC, m_pLoop, this);
    struct sockaddr_in addr = { 0 };
    if (0 != uv_ip4_addr(m_pConfig->IPAddrV4().c_str(), m_pConfig->Port(), &addr))
     break;
    if (0 != uv_udp_bind(T_GET_HANDLE<uv_udp_t>(m_pServer), (const struct sockaddr*)&addr, 0))
     break;
    if (0 != uv_udp_recv_start(T_GET_HANDLE<uv_udp_t>(m_pServer), OnUdpAlloc, OnUdpRead))
     break;
    break;
   }break;
   default:
    break;
   }

   notify_parent_process();
   m_Threads.emplace_back([this]() {Process(); });
   m_Threads.emplace_back(
    [this]() {
     uv_run((uv_loop_t*)m_pLoop->Handle(), UV_RUN_DEFAULT);
    });
   LOG_OUTPUT(std::format("Server module start success on [{}:{}].", m_pConfig->IPAddrV4(), m_pConfig->Port()));
  } while (0);

  return m_IsOpen.load();
 }
 void Server::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);

   T_CLOSE_HANDLE(m_pServer, m_pAsync);
   m_Sessions.iterate(
    [&](const auto&, Session* session, bool&) {
     session->ForceClose();
    });
   do {
    if (m_Sessions.empty())
     break;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   } while (1);
   T_CLOSE_HANDLE(m_pAsync, m_pAsync);
   m_pLoop->Close();

   for (auto& it : m_Threads)
    it.join();
   m_Threads.clear();
  } while (0);
 }

 //!@ private
 void Server::Process() {
  LOG_OUTPUT(std::format("Start server work thread."));

  const time_t ThreadCycleInterval = 10;//ms

  switch (m_SessionType) {
  case SessionType::UDP_SESSION_SERVER: {
   do {


    std::this_thread::sleep_for(std::chrono::milliseconds(ThreadCycleInterval));
    if (!m_IsOpen.load())
     break;
   } while (1);
  }break;
  default: {
   do {
    const std::uint64_t time_current_ms = shared::Win::Time::TimeStamp<std::chrono::milliseconds>();
    m_Sessions.iterate_clear(
     [&](const TypeIdentify& identify, Session* pSession, bool& itbreak, bool& itclear) {
      const auto session_status = pSession->Status();
      switch (session_status) {
      case SessionStatus::Closing: {

      }break;
      case SessionStatus::ForceClose: {
       pSession->Stop();
      }break;
      case SessionStatus::Unknown/*Unready*/:
       [[fallthrough]];
      case SessionStatus::Stopped: {
       LOG_OUTPUT(std::format("Session logout on({})", pSession->Address()));
       pSession->Release();
       itclear = true;
      }break;
      case SessionStatus::Started: {
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

         LOG_OUTPUT(
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
      }break;
      default:
       break;
      }
     });
    std::this_thread::sleep_for(std::chrono::milliseconds(ThreadCycleInterval));
    if (!m_IsOpen.load() && m_Sessions.empty())
     break;
   } while (1);
  }break;
  }///swtich


  LOG_OUTPUT(std::format("Exited server work thread."));
 }

 void Server::PushSession(const TypeIdentify& identify, Session* pSession) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Sessions.push(identify, pSession);
 }
 UvHandle* Server::Handle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pServer;
 }
 const SessionType& Server::Type() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_SessionType;
 }
 void Server::OnConnect(uv_stream_t* server, int status) {
  do {
   if (!server || status != 0)
    break;
   auto caller = T_GET_HANDLE_CALLER<Server>(server);
   if (!caller)
    break;
   if (status != 0)
    break;
   SessionType type = caller->Type();
   switch (type) {
   case SessionType::TCP_SESSION_SERVER: {
    auto pSession = new Session(type, caller);
    if (pSession->Identify() <= 0)
     break;
    if (!pSession->Start())
     break;
   }break;
   case SessionType::IPC_SESSION_SERVER: {
    auto pSession = new Session(type, caller);
    if (pSession->Identify() <= 0)
     break;
    if (!pSession->Start())
     break;
   }break;
   default:
    break;
   }
  } while (0);
 }
 void Server::OnUdpAlloc(uv_handle_t* server, size_t suggested_size, uv_buf_t* buf) {
  buf->base = reinterpret_cast<char*>(malloc(suggested_size));
  buf->len = suggested_size;
 }
 void Server::OnUdpRead(uv_udp_t* server, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
  auto pServer = T_GET_HANDLE_CALLER<Server>(server);
  do {
   if (!pServer)
    break;
   if (!addr || !buf)
    break;
   if (nread <= 0)
    break;


#if 0
   auto identify = Protocol::make_sock_session_identify(SessionType::UDP_SESSION_SERVER, *addr);
   if (identify <= 0)
    break;
   auto pSession = new Session(SessionType::UDP_SESSION_SERVER, pServer);
   pSession->SockAddr(addr);
   pSession->Identify(identify);
   auto found = pServer->m_Sessions.search(identify);
   if (found) {
    pSession->Stop();
    SK_DELETE_PTR(pSession);
    pSession = *found;
   }
   else {
    pServer->PushSession(pSession->Identify(), pSession);
   }
   if (!pSession->Read(buf->base, nread))
    break;
   if (!pSession->Start())
    break;
#endif
  } while (0);

  if (nread < 0 && pServer) {

  }
  if (buf)
   free(buf->base);
 }

 Server* __gpServer = nullptr;

}///namespace local
