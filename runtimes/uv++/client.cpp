#include "stdafx.h"

namespace local {
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
 Client::Client(const unsigned long& identify)
  : m_Identify(identify) {
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
 void Client::Init() {
  m_pConfig = new Config();
 }
 void Client::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }
 size_t Client::SessionCount() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return 1;
 }
 bool Client::IsClose() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return !m_IsOpen.load();
 }
 Config* Client::ConfigGet() const {
  return m_pConfig;
 }
 bool Client::Start() {
  do {
   if (m_IsOpen.load())
    break;
   if (m_pConfig->EnableClientWaitForTheInitialConnectionAndReceiveResult())
    m_hFirstLogicalEvent = ::CreateEventW(NULL, TRUE, FALSE, NULL);

   m_IsOpen.store(true);
   if (0 != uv_thread_create(&thread_main_, MainProcess, this)) {
    m_IsOpen.store(false);
    break;
   }

   if (!m_IsOpen.load())
    break;

   if (m_hFirstLogicalEvent)
    ::WaitForSingleObject(m_hFirstLogicalEvent, INFINITE);

  } while (0);
  return m_IsOpen.load();
 }
 void Client::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   if (m_hFirstLogicalEvent) {
    ::SetEvent(m_hFirstLogicalEvent);
    ::WaitForSingleObject(m_hFirstLogicalEvent, INFINITE);
   }
   SK_CLOSE_HANDLE(m_hFirstLogicalEvent);
   m_IsOpen.store(false);
   uv_thread_join(&thread_main_);
  } while (0);
 }
 bool Client::Write(const CommandType& cmd, const char* data, const size_t& len) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!data || len <= 0)
    break;
   m_WriteBufferQ.push(cmd, std::string(data, len));
   result = true;
  } while (0);
  return result;
 }
 void Client::NotifyFirstLogicalEvent() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_hFirstLogicalEvent && !m_FirstLogicalEventTriggerFlag.load()) {
   ::SetEvent(m_hFirstLogicalEvent);
   m_FirstLogicalEventTriggerFlag.store(true);
  }
 }
 ServerStatus Client::Status() const {
  return m_ServerStatus;
 }
 ServerType Client::ServerTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return Protocol::GetServerType(m_Identify);;
 }
 SessionType Client::SessionTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return Protocol::GetSessionType(m_Identify);
 }
 AddressType Client::AddressTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return Protocol::GetAddressType(m_Identify);
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

   switch (pClient->SessionTypeGet()) {
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

   client->data = new Session;
   if (connect)
    connect->data = client->data;
   USERDATA_PTR(loop)->Caller(arg);
   USERDATA_PTR(client)->Caller(arg);
   USERDATA_PTR(client)->Handle(client);
  } while (0);
 }
 void Client::MainProcess(void* arg) {
  assert(arg);
  Client* pClient = reinterpret_cast<Client*>(arg);
  uv_loop_t* loop = nullptr;
  uv_handle_t* client = nullptr;
  uv_handle_t* connect = nullptr;
  unsigned long long prev_time_ms_reconn = 0;
  unsigned long long prev_time_ms_keepalive = 0;
  SessionStatus status = SessionStatus::UNKNOWN;
  do {
   if (client)
    status = USERDATA_PTR(client)->Status();

   const unsigned long long time_current_ms = shared::TimeStampUTC<std::chrono::milliseconds>();
   const unsigned long long time_reconnection_interval = time_current_ms - prev_time_ms_reconn;

   do {
    if (time_reconnection_interval < pClient->ConfigGet()->ClientReconnectionIntervalMS())
     break;
    prev_time_ms_reconn = time_current_ms;

    switch (status) {
    case SessionStatus::UNKNOWN:
     [[fallthrough]];
    case SessionStatus::DISCONNECTED: {
     prev_time_ms_keepalive = 0;
     if (!loop || !client || !connect) {
      uv_main_init(arg, loop, client, connect);
     }
     std::string connection_address = pClient->ConfigGet()->Address();
     pClient->OnHookConnection(connection_address);
     pClient->ConfigGet()->Address(connection_address);

     Connect(pClient->ConfigGet()->Address(), connect, client);
     LOG_OUTPUT("Preparing to connect to the service ({}).", pClient->ConfigGet()->Address());
    }break;
    case SessionStatus::FORCECLOSE: {
     prev_time_ms_keepalive = 0;
     pClient->OnForceClose(client ? SESSION_PTR(client) : nullptr);
     LOG_OUTPUT("Disconnect from the service ({}).", pClient->ConfigGet()->Address());
     uv_main_uninit(arg, loop, client, connect);
     uv_main_init(arg, loop, client, connect);
    }break;
    default:
     break;
    }

   } while (0);


   if (status == SessionStatus::CONNECTED && client) {
    auto pSession = SESSION_PTR(client);

    do {
     if (pClient->m_WriteBufferQ.empty())
      break;
     pClient->m_WriteBufferQ.iterate_clear(
      [&](const auto& cmd, auto& data, bool&, bool& itclear) {
       pSession->Write(cmd, data);
       itclear = true;
      });
    } while (0);


    do {//!@ keepalive
     if (prev_time_ms_keepalive <= 0) {
      prev_time_ms_keepalive = time_current_ms;
      break;
     }
     if (time_current_ms - prev_time_ms_keepalive < pClient->ConfigGet()->KeepAliveTimeMS())
      break;
     prev_time_ms_keepalive = time_current_ms;
     if (!pSession)
      break;

     std::string keepalive_send_data = R"(#---!)";
     pClient->OnHookKeepAliveSend(pSession,
      [&](const char** route, size_t* route_size) {
       if (*route && *route_size > 0) {
        keepalive_send_data.clear();
        keepalive_send_data.append(const_cast<char*>(*route), *route_size);
       }
      });
     if (!pSession->Write(CommandType::KEEPALIVE, keepalive_send_data))
      pSession->ForceClose();
    } while (0);

    do {//!@ write
     write_req_t* req = pSession->Write(
      [&](std::string& message) {
       pClient->OnHookWrite(pSession, message);
      });
     if (!req)
      break;
     req->handle = client;
     if (0 != uv_write(&req->write, reinterpret_cast<uv_stream_t*>(req->handle), &req->buf, 1, Protocol::uv_write_cb)) {
      pClient->OnSystemMessage(pSession,
       SystemErrorno::E_STREAM_SEND, \
       (req->buf.base && req->buf.len > 0) ? std::string(req->buf.base, req->buf.len) : "");
      SK_DELETE_PTR(req);
      pSession->ForceClose();
      break;
     }

     prev_time_ms_keepalive = time_current_ms;
    } while (0);

    do {//read
     std::string read_data = pSession->Read();
     if (read_data.empty())
      break;
     HEAD head;
     std::string message;
     if (!Protocol::UnMakeStream(read_data, head, message)) {
      pClient->OnSystemMessage(pSession, SystemErrorno::E_STREAM_RECV, read_data);
      pSession->ForceClose();
      break;
     }
     pClient->OnMessage(pSession, head.Command(), message);
     {
      CommandType cmdReply = CommandType::UNKNOWN;
      std::string messageReply;
      pClient->OnReceiveReply(pSession, head.Command(), message, cmdReply,
       [&](const char** route, size_t* route_size) {
        if (*route && *route_size > 0) {
         messageReply.clear();
         messageReply.append(const_cast<char*>(*route), *route_size);
        }
       });
      if (CommandType::UNKNOWN != cmdReply) {
       if (!pSession->Write(cmdReply, messageReply))
        pSession->ForceClose();
      }
     }

     //LOG_OUTPUT(std::format("recv message({:X}:{})", static_cast<unsigned long>(head.Command()), message));
     switch (head.Command()) {
     case CommandType::WELCOME: {
      std::string replyWelcome;
      pClient->OnWelcome(pSession, message,
       [&](const char** route, size_t* route_size) {
        if (*route && *route_size > 0) {
         replyWelcome.clear();
         replyWelcome.append(const_cast<char*>(*route), *route_size);
        }
       });
      if (!replyWelcome.empty()) {
       if (!pSession->Write(CommandType::HELLO, replyWelcome)) {
        pSession->ForceClose();
       }
      }
     }break;
     default:
      break;
     }
     //!@ 等待第一次接收处理完成
     pClient->NotifyFirstLogicalEvent();
    } while (0);

   }


   if (loop) {
    uv_run(loop, uv_run_mode::UV_RUN_NOWAIT);
   }
   if (pClient->IsClose()) {
    bool is_close = pClient->IsClose();
    pClient->OnHookSystemExit(is_close);
    if (is_close)
     break;
   }
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (1);
  uv_main_uninit(arg, loop, client, connect);
  pClient->OnSystemExit();
 }
 bool Client::Connect(const std::string& address, uv_handle_t* connect, uv_handle_t* client) {
  bool result = false;
  Client* pClient = nullptr;
  Session* pSession = nullptr;
  do {
   if (address.empty() || !connect || !client)
    break;
   if (uv_is_closing(client))
    break;
   pSession = SESSION_PTR(client);
   if (!pSession)
    break;
   pClient = reinterpret_cast<Client*>(pSession->Caller());
   if (!pClient)
    break;
   if (pSession->Status() == SessionStatus::CONNECTING)
    break;
   pSession->Status(SessionStatus::CONNECTING);

   switch (pClient->SessionTypeGet()) {
   case SessionType::TCP: {
    std::string sockaddr_buffer;
    if (!Protocol::MakeIPAddr(pClient->ConfigGet()->Address(), sockaddr_buffer, pClient->AddressTypeGet()))
     break;
    if (0 != uv_tcp_connect(reinterpret_cast<uv_connect_t*>(connect), \
     reinterpret_cast<uv_tcp_t*>(client), (const struct sockaddr*)sockaddr_buffer.data(), Client::ConnectCb))
     break;
    result = true;
   }break;
   case SessionType::IPC: {
    uv_pipe_connect(
     reinterpret_cast<uv_connect_t*>(connect),
     reinterpret_cast<uv_pipe_t*>(client),
     pClient->ConfigGet()->Address().c_str(),
     Client::ConnectCb);
    result = true;
   }break;
   case SessionType::UDP: {
    std::string sockaddr_buffer;
    if (!Protocol::MakeIPAddr(pClient->ConfigGet()->Address(), sockaddr_buffer, pClient->AddressTypeGet()))
     break;
    pSession->SockAddr(*reinterpret_cast<LPSOCKADDR>(sockaddr_buffer.data()));
    if (0 != uv_udp_connect(reinterpret_cast<uv_udp_t*>(client), reinterpret_cast<LPSOCKADDR>(sockaddr_buffer.data())))
     break;
    if (0 != uv_udp_recv_start(reinterpret_cast<uv_udp_t*>(client), uv_udp_alloc_cb, uv_udp_recv_cb))
     break;
    result = true;
   }break;
   }

   if (!result)
    break;
   result = false;

   {



   }

   result = true;
  } while (0);
  if (!result && pSession)
   pSession->ForceClose();
  return result;
 }

 void Client::ConnectCb(uv_connect_t* req, int status) {
  bool success = false;
  Session* pSession = nullptr;
  Client* pClient = nullptr;
  do {
   pSession = SESSION_PTR(req);
   if (!pSession)
    break;
   pClient = reinterpret_cast<Client*>(pSession->Caller());
   if (!pClient)
    break;
   if (status < 0) {
    pClient->OnDisconnection(pSession);
    pClient->NotifyFirstLogicalEvent();
    break;
   }
   if (0 != uv_read_start(reinterpret_cast<uv_stream_t*>(pSession->Handle()), Protocol::uv_alloc_cb, uv_read_cb))
    break;
   pClient->OnConnection(pSession);
   pSession->Status(SessionStatus::CONNECTED);
   LOG_OUTPUT("Connect to the service({}) complete.", pClient->ConfigGet()->Address());
   success = true;
  } while (0);
  if (!success && pSession)
   pSession->ForceClose();
 }

 void uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (!stream)
   return;
  bool force_close = false;
  Session* pSession = USERDATA_PTR(stream);
  if (buf->base) {
   if (nread > 0) {
    if (!pSession->Read(buf->base, nread))
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
   pSession->ForceClose();
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

}///namespace local