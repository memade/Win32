#include "stdafx.h"

namespace local {
 uv_thread_t thread_main_ = nullptr;
 void uv_main(void*);
 void uv_connect(const std::string&, uv_handle_t*, uv_handle_t*);
 void uv_connect_cb(uv_connect_t* req, int status);
 void uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
 void uv_udp_alloc_cb(uv_handle_t* handle,
  size_t suggested_size,
  uv_buf_t* buf);
 void uv_udp_recv_cb(uv_udp_t* handle,
  ssize_t nread,
  const uv_buf_t* buf,
  const struct sockaddr* addr,
  unsigned flags);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
 Client::Client() {
  LOG_INIT;
  Init();
 }
 Client::~Client() {
  UnInit();
  LOG_UNINIT;
 }
 void Client::Release() const {
  delete this;
 }
 ServerStatus Client::Status() const {
  return m_ServerStatus;
 }
 void Client::Init() {
  m_pConfig = new Config();
  m_pSession = new Session();
 }
 void Client::UnInit() {
  SK_DELETE_PTR(m_pConfig);
  SK_DELETE_PTR(m_pSession);
 }
 bool Client::Close() const {
  bool result = false;
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  result = m_IsOpen.load() == false;
  lock.unlock();
  return result;
 }
 Config* Client::ConfigGet() const {
  return m_pConfig;
 }
 bool Client::Start() {
  do {
   if (m_IsOpen.load())
    break;
   if (0 != uv_thread_create(&thread_main_, uv_main, this))
    break;
   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void Client::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   uv_thread_join(&thread_main_);
  } while (0);
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
 static void uv_main_uninit(void* arg, uv_loop_t*& loop, uv_handle_t*& client, uv_handle_t*& connect) {
  do {
   if (!arg || !loop || !client)
    break;
   uv_close(client, Protocol::uv_close_cb);
   uv_walk(loop, Protocol::walk_close_handle_cb, arg);
   uv_run(loop, uv_run_mode::UV_RUN_DEFAULT);
   uv_loop_close(loop);
  } while (0);

  client = nullptr;

  SK_DELETE_PTR(connect);

  if (loop) {
   auto udata = USERDATA_PTR(loop);
   SK_DELETE_PTR(udata);
   SK_DELETE_PTR(loop);
  }

 }
 static void uv_main_init(void* arg, uv_loop_t*& loop, uv_handle_t*& client, uv_handle_t*& connect) {
  auto pClient = reinterpret_cast<Client*>(arg);
  do {
   loop = new uv_loop_t;
   loop->data = new UserData;
   if (0 != uv_loop_init(loop))
    break;

   switch (pClient->ConfigGet()->Session()) {
   case SessionType::TCP: {
    client = reinterpret_cast<uv_handle_t*>(new uv_tcp_t);
    connect = reinterpret_cast<uv_handle_t*>(new uv_connect_t);
    if (0 != uv_tcp_init(loop, reinterpret_cast<uv_tcp_t*>(client)))
     break;

   }break;
   case SessionType::IPC: {
    client = reinterpret_cast<uv_handle_t*>(new uv_pipe_t);
    connect = reinterpret_cast<uv_handle_t*>(new uv_connect_t);
    if (0 != uv_pipe_init(loop, reinterpret_cast<uv_pipe_t*>(client), 1))
     break;

   }break;
   case SessionType::UDP: {
    client = reinterpret_cast<uv_handle_t*>(new uv_udp_t);
    if (0 != uv_udp_init(loop, reinterpret_cast<uv_udp_t*>(client)))
     break;

   }break;
   default:
    break;
   }

   client->data = new UserData;
   if(connect)
    connect->data = client->data;
   USERDATA_PTR(loop)->Caller(arg);
   USERDATA_PTR(client)->Caller(arg);
   USERDATA_PTR(client)->Handle(client);
  } while (0);
 }
 void uv_main(void* arg) {
  Client* pClient = reinterpret_cast<Client*>(arg);
  uv_loop_t* loop = nullptr;
  uv_handle_t* client = nullptr;
  uv_handle_t* connect = nullptr;
  unsigned long long current_time_ms = 0;
  unsigned long long prev_time_ms = 0;
  SessionStatus status = SessionStatus::UNKNOWN;
  do {
   if (client)
    status = USERDATA_PTR(client)->Status();

   current_time_ms = shared::TimeStampUTC<std::chrono::milliseconds>();
   if (current_time_ms - prev_time_ms >= pClient->ConfigGet()->ClientReconnectionIntervalMS()) {

    switch (status) {
    case SessionStatus::UNKNOWN:
     [[fallthrough]];
    case SessionStatus::DISCONNECTED: {
     if (!loop || !client || !connect)
      uv_main_init(arg, loop, client, connect);
     uv_connect(pClient->ConfigGet()->Address(), connect, client);
     LOG_OUTPUT(std::format("Preparing to connect to the service ({}).", pClient->ConfigGet()->Address()));
    }break;
    case SessionStatus::FORCECLOSE: {
     LOG_OUTPUT(std::format("Disconnect from the service ({}).", pClient->ConfigGet()->Address()));
     uv_main_uninit(arg, loop, client, connect);
     uv_main_init(arg, loop, client, connect);
    }break;
    default:
     break;
    }

    prev_time_ms = current_time_ms;
   }

   if (status == SessionStatus::CONNECTED && client) {
    auto pSession = SESSION_PTR(client);

    do {//!@ write
     write_req_t* req = pSession->Write();
     if (!req)
      break;
     req->handle = client;
     if (0 != uv_write(&req->write, reinterpret_cast<uv_stream_t*>(req->handle), &req->buf, 1, Protocol::uv_write_cb)) {
      SK_DELETE_PTR(req);
      pSession->Status(SessionStatus::FORCECLOSE);
      break;
     }
    } while (0);

    do {//read
     std::string read_data = pSession->Read();
     if (read_data.empty())
      break;
     HEAD head;
     std::string message;
     if (!Protocol::UnMakeStream(read_data, head, message)) {
      pSession->Status(SessionStatus::FORCECLOSE);
      break;
     }
     LOG_OUTPUT(std::format("recv message({:X}:{})", static_cast<unsigned long>(head.Command()), message));

     switch (head.Command()) {
     case CommandType::WELCOME: {
      if (!pSession->Write(CommandType::HELLO, std::format("Hello!")))
       pSession->Status(SessionStatus::FORCECLOSE);
     }break;
     default:
      break;
     }
    } while (0);



   }


   if (loop) {
    uv_run(loop, uv_run_mode::UV_RUN_NOWAIT);
   }
   if (pClient->Close())
    break;
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (1);
  uv_main_uninit(arg, loop, client, connect);
 }
 void uv_connect(const std::string& address, uv_handle_t* connect, uv_handle_t* client) {
  bool bClose = true;
  auto pSession = SESSION_PTR(client);
  auto pClient = reinterpret_cast<Client*>(pSession->Caller());
  do {
   if (!pSession ||!pClient)
    break;
   if (pSession->Status() == SessionStatus::CONNECTING)
    break;
   pSession->Status(SessionStatus::CONNECTING);
   if (!client || uv_is_closing(client))
    break;

   switch (pClient->ConfigGet()->Session()) {
   case SessionType::TCP: {
    std::string ip;
    unsigned short port = 0;
    Protocol::parser_ipaddr(address, ip, port);
    struct sockaddr_in addr = { 0 };
    if (uv_ip4_addr(ip.c_str(), port, &addr) != 0)
     break;
    if (0 != uv_tcp_connect(reinterpret_cast<uv_connect_t*>(connect), reinterpret_cast<uv_tcp_t*>(client), (const struct sockaddr*)&addr, uv_connect_cb))
     break;
    bClose = false;
   }break;
   case SessionType::IPC: {
    uv_pipe_connect(
     reinterpret_cast<uv_connect_t*>(connect),
     reinterpret_cast<uv_pipe_t*>(client),
     pClient->ConfigGet()->Address().c_str(),
     uv_connect_cb);
    bClose = false;
   }break;
   case SessionType::UDP: {
    struct sockaddr_in addr = { 0 };
    std::string ip;
    u_short port = 0;
    if (!Protocol::parser_ipaddr(address, ip, port))
     break;
    if (uv_ip4_addr(ip.c_str(), port, &addr) != 0)
     break;
    pSession->SockAddr(addr);
    if (0 != uv_udp_connect(reinterpret_cast<uv_udp_t*>(client), (const struct sockaddr*)&addr))
     break;
    if (0 != uv_udp_recv_start(reinterpret_cast<uv_udp_t*>(client), uv_udp_alloc_cb, uv_udp_recv_cb))
     break;
    bClose = false;
   }break;
   }

  } while (0);
  if (bClose && pSession) {
   pSession->Status(SessionStatus::FORCECLOSE);
  }
 }

 void uv_connect_cb(uv_connect_t* req, int status) {
  auto pSession = SESSION_PTR(req);
  auto pClient = reinterpret_cast<Client*>(pSession->Caller());
  pSession->Status(SessionStatus::FORCECLOSE);
  do {
   if (status < 0)
    break;
   if (0 != uv_read_start(reinterpret_cast<uv_stream_t*>(pSession->Handle()), Protocol::uv_alloc_cb, uv_read_cb))
    break;
   pSession->Status(SessionStatus::CONNECTED);
   LOG_OUTPUT(std::format("Connect to the service({}) complete.", pClient->ConfigGet()->Address()));
  } while (0);

 }

 void uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (!stream)
   return;
  bool force_close = false;
  auto udata = USERDATA_PTR(stream);
  if (buf->base) {
   if (nread > 0) {
    if (!udata->Read(buf->base, nread))
     force_close = true;
   }
   else if (nread < 0) {
    force_close = true;
   }
  }
  else {
   force_close = true;
  }
  if (force_close)
   udata->Status(SessionStatus::FORCECLOSE);
 }

 void uv_udp_alloc_cb(uv_handle_t* handle,
  size_t suggested_size,
  uv_buf_t* buf) {

  auto sk = 0;
 }
 void uv_udp_recv_cb(uv_udp_t* handle,
  ssize_t nread,
  const uv_buf_t* buf,
  const struct sockaddr* addr,
  unsigned flags) {

  auto sk = 0;
 }


 Client* __gpClient = nullptr;
}///namespace local