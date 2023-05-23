#include "stdafx.h"

namespace local {
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void udp_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
 void udp_recv_cb(uv_udp_t* handle,
  ssize_t nread,
  const uv_buf_t* buf,
  const struct sockaddr* addr,
  unsigned flags);
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Server::Server() {
  LOG_INIT;
  Init();
 }

 Server::~Server() {
  UnInit();
  LOG_UNINIT;
 }
 void Server::Release() const {
  delete this;
 }
 void Server::Init() {
  m_pConfig = new Config();
 }
 void Server::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }
 bool Server::IsClose() const {
  bool result = false;
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  result = m_IsOpen.load() == false;
  lock.unlock();
  return result;
 }
 Config* Server::ConfigGet() const {
  Config* result = nullptr;
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  result = m_pConfig;
  lock.unlock();
  return result;
 }
 bool Server::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_ServerStatus = ServerStatus::STARTING;
   if (0 != uv_thread_create(&thread_main_, MainProcess, this))
    break;
   m_ServerStatus = ServerStatus::STARTED;
   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }

 void Server::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_ServerStatus = ServerStatus::STOPPING;
   m_IsOpen.store(false);
   uv_thread_join(&thread_main_);
   m_ServerStatus = ServerStatus::STOPPED;
  } while (0);
 }
 void Server::Status(const ServerStatus& status) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  m_ServerStatus = status;
  lock.unlock();
 }
 ServerStatus Server::Status() const {
  ServerStatus result = ServerStatus::UNKNOWN;
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  result = m_ServerStatus;
  lock.unlock();
  return result;
 }

 void Server::SessionCount(const unsigned long& count) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  m_SessionCount.store(count);
  lock.unlock();
 }
 unsigned long Server::SessionCount() const {
  unsigned long result = 0;
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  result = m_SessionCount.load();
  lock.unlock();
  return result;
 }
 void Server::MainProcess(void* arg) {
  Server* pServer = reinterpret_cast<Server*>(arg);
  if (!pServer)
   return;
  uv_loop_t* loop = nullptr;
  uv_handle_t* server = nullptr;
  do {
   loop = new uv_loop_t;
   loop->data = new UserData;
   USERDATA_PTR(loop)->Caller(arg);
   if (!loop)
    break;
   if (0 != uv_loop_init(loop))
    break;

   std::string server_start_address = pServer->ConfigGet()->Address();
   pServer->OnHookServerAddress(server_start_address);

   switch (pServer->ConfigGet()->Session()) {
   case SessionType::TCP: {
    server = (uv_handle_t*)new uv_tcp_t;
    if (!server)
     break;
    if (0 != uv_tcp_init(loop, (uv_tcp_t*)server))
     break;
    std::string ip;
    unsigned short port = 0;
    if (!Protocol::parser_ipaddr(server_start_address, ip, port))
     break;
    struct sockaddr_in addr = { 0 };
    if (0 != uv_ip4_addr(ip.c_str(), port, &addr))
     break;
    if (0 != uv_tcp_bind((uv_tcp_t*)server, (const struct sockaddr*)&addr, 0))
     break;
    if (0 != uv_listen((uv_stream_t*)server, SOMAXCONN, Server::SessionConnectionCb))
     break;
    pServer->Status(ServerStatus::READY);
   }break;
   case SessionType::IPC: {
    server = (uv_handle_t*)new uv_pipe_t;
    if (!server)
     break;
    if (0 != uv_pipe_init(loop, (uv_pipe_t*)server, 0))
     break;
    if (0 != uv_pipe_bind((uv_pipe_t*)server, server_start_address.c_str()))
     break;
    if (0 != uv_listen((uv_stream_t*)server, SOMAXCONN, Server::SessionConnectionCb))
     break;
    pServer->Status(ServerStatus::READY);
   }break;
   case SessionType::UDP: {
    server = (uv_handle_t*)new uv_udp_t;
    if (!server)
     break;
    if (0 != uv_udp_init(loop, (uv_udp_t*)server))
     break;
    std::string ip;
    unsigned short port = 0;
    if (!Protocol::parser_ipaddr(server_start_address, ip, port))
     break;
    struct sockaddr_in addr = { 0 };
    if (0 != uv_ip4_addr(ip.c_str(), port, &addr))
     break;
    if (0 != uv_udp_bind(reinterpret_cast<uv_udp_t*>(server), (const struct sockaddr*)&addr, 0))
     break;
    if (0 != uv_udp_recv_start(reinterpret_cast<uv_udp_t*>(server), udp_alloc_cb, udp_recv_cb))
     break;
    pServer->Status(ServerStatus::READY);
   }break;
   default:
    break;
   }

   if (pServer->Status() != ServerStatus::READY || !server) {
    pServer->OnSystemMessage(nullptr, SystemErrorno::E_SYSTEM_CORE_INIT, "");
    break;
   }
   server->data = new UserData;
   USERDATA_PTR(server)->Caller(arg);
   USERDATA_PTR(server)->Handle(server);
   USERDATA_PTR(server)->ServerTypeSet(ServerType::ACCEPTOR);

   pServer->OnServerReady();

   do {
    uv_walk(loop, Server::WorkProcess, arg);
    uv_run(loop, uv_run_mode::UV_RUN_NOWAIT);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (pServer->IsClose()) {
     bool is_close = pServer->IsClose();
     pServer->OnHookSystemExit(is_close);
     if (is_close) {
      uv_close(server, Protocol::uv_close_cb);
      uv_walk(loop, Protocol::walk_close_handle_cb, arg);
      uv_run(loop, uv_run_mode::UV_RUN_DEFAULT);
      uv_loop_close(loop);
      break;
     }
    }
   } while (1);

  } while (0);

  if (loop) {
   auto udata = USERDATA_PTR(loop);
   SK_DELETE_PTR(udata);
   SK_DELETE_PTR(loop);
  }

  pServer->OnSystemExit();
 }


 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void Server::SessionConnectionCb(uv_stream_t* server, int status) {
  if (status != 0 || !server)
   return;
  auto pServerUserData = USERDATA_PTR(reinterpret_cast<uv_handle_t*>(server));
  if (!pServerUserData)
   return;
  auto pServer = reinterpret_cast<Server*>(pServerUserData->Caller());
  if (!pServer)
   return;
  Session* pSession = nullptr;
  bool success = false;
  uv_handle_t* client = nullptr;


  std::string client_address;
  switch (server->type) {
  case uv_handle_type::UV_TCP: {
   client = reinterpret_cast<uv_handle_t*>(new uv_tcp_t);
   if (!client)
    break;
   if (0 != uv_tcp_init(server->loop, reinterpret_cast<uv_tcp_t*>(client)))
    break;
   if (0 != uv_accept(server, reinterpret_cast<uv_stream_t*>(client)))
    break;
   if (0 != uv_read_start(reinterpret_cast<uv_stream_t*>(client), Protocol::uv_alloc_cb, Protocol::uv_recv_cb))
    break;
   struct sockaddr addr = { 0 };
   int addr_len = sizeof(struct sockaddr);
   if (0 != uv_tcp_getpeername(reinterpret_cast<uv_tcp_t*>(client), &addr, &addr_len))
    break;
   if (!Protocol::parser_ipaddr(&addr, client_address))
    break;
   success = true;
  }break;
  case uv_handle_type::UV_NAMED_PIPE: {
   client = reinterpret_cast<uv_handle_t*>(new uv_pipe_t);
   if (!client)
    break;
   if (0 != uv_pipe_init(server->loop, reinterpret_cast<uv_pipe_t*>(client), 1))
    break;
   if (0 != uv_accept(server, reinterpret_cast<uv_stream_t*>(client)))
    break;
   char name[1024] = { 0 };
   size_t name_len = sizeof(name);
   if (0 != uv_pipe_getpeername(reinterpret_cast<uv_pipe_t*>(client), name, &name_len))
    break;
   client_address = name;
   if (0 != uv_read_start(reinterpret_cast<uv_stream_t*>(client), Protocol::uv_alloc_cb, Protocol::uv_recv_cb))
    break;
   success = true;
  }break;
  case uv_handle_type::UV_UDP: {
   client = reinterpret_cast<uv_handle_t*>(new uv_udp_t);
   success = true;
  }break;
  default:
   break;
  }

  if (!client) {
   pServer->OnSystemMessage(nullptr, SystemErrorno::E_SERVER_SESSION_INIT, "");
   return;
  }

  client->data = new Session;
  pSession = SESSION_PTR(client);
  pSession->Caller(pServer);
  pSession->Handle(client);
  pSession->ServerTypeSet(ServerType::INITIATOR);
  pSession->Address(client_address);
  pSession->Status(success ? SessionStatus::STARTED : SessionStatus::FORCECLOSE);
  pServer->OnHookSessionCreate(pSession);
  pServer->OnAccept(pSession, success);

  if (!success)
   return;

  pServer->OnServerSessionReady(pSession);

  do {
   std::string welcome = "Welcome to server!";
   pServer->OnHookWelcomeSend(pSession, welcome);
   if (welcome.empty())
    break;
   if (pSession->Write(CommandType::WELCOME, welcome))
    break;
   pSession->ForceClose();
  } while (0);

 }

 void Server::WorkProcess(uv_handle_t* client, void* arg) {
  if (uv_is_closing(client) || !client)
   return;
  Server* pServer = reinterpret_cast<Server*>(arg);
  if (!pServer)
   return;
  Session* pSession = SESSION_PTR(client);
  const unsigned long long current_time_ms = shared::TimeStampUTC<std::chrono::milliseconds>();

  do {
   if (!pSession)
    break;

   switch (pSession->ServerTypeGet()) {
   case ServerType::UNKNOWN:
    [[fallthrough]];
   case ServerType::ACCEPTOR:
    break;
   case ServerType::INITIATOR: {
    const auto session_status = pSession->Status();

    switch (session_status) {
    case SessionStatus::FORCECLOSE: {
     if (uv_is_closing(client))
      break;
     pServer->OnHookSessionDestory(pSession);
     uv_close(client, Protocol::uv_close_cb);
    }break;
    case SessionStatus::STARTED: {
     unsigned long long timeout_ms = pSession->ActivationTime(current_time_ms);
     if (timeout_ms >= pServer->ConfigGet()->SessionTimeoutMS()) {
      pServer->OnServerSessionTimeout(pSession, timeout_ms);
      pSession->ForceClose();
      break;
     }

     do {//!@ write
      write_req_t* req = pSession->Write(
       [&](std::string& write_data) {
        pServer->OnHookWrite(pSession, write_data);
       });
      if (!req)
       break;
      req->handle = reinterpret_cast<decltype(req->handle)>(client);
      if (0 != uv_write(&req->write, reinterpret_cast<uv_stream_t*>(req->handle), &req->buf, 1, Protocol::uv_write_cb)) {
       pServer->OnSystemMessage(pSession, SystemErrorno::E_STREAM_SEND, \
        (req->buf.base && req->buf.len > 0) ? std::string(req->buf.base, req->buf.len) : "");
       SK_DELETE_PTR(req);
       pSession->ForceClose();
       break;
      }
     } while (0);

     do {//!@ read
      std::string data = pSession->Read();
      if (data.empty())
       break;
      HEAD head;
      std::string message;
      if (!Protocol::UnMakeStream(data, head, message)) {
       pServer->OnSystemMessage(pSession, SystemErrorno::E_STREAM_RECV, data);
       pSession->Status(SessionStatus::FORCECLOSE);
       break;
      }

      pServer->OnMessage(pSession, head.Command(), message);
      CommandType cmdReply = CommandType::UNKNOWN;
      std::string messageReply;
      pServer->OnReceiveReply(pSession, head.Command(), message, cmdReply, messageReply);
      if (CommandType::UNKNOWN != cmdReply) {
       if (!pSession->Write(cmdReply, messageReply))
        pSession->Status(SessionStatus::FORCECLOSE);
      }

      switch (head.Command()) {
      case CommandType::KEEPALIVE: {
       std::string message_reply;
       pServer->OnKeepAlive(pSession, message, message_reply);
       if (message_reply.empty())
        break;
       if (!pSession->Write(CommandType::HEARTBEAT, message_reply))
        pSession->ForceClose();
      }break;
      case CommandType::HELLO: {
       std::string replyHello;
       pServer->OnHello(pSession, message, replyHello);
       if (replyHello.empty())
        break;
       if (!pSession->Write(CommandType::WELCOME, replyHello))
        pSession->ForceClose();
      }break;
      default:
       break;
      }

      pSession->ActivationTimeUpdate(current_time_ms);
     } while (0);

    }break;
    default:
     break;
 }///switch




}break;///case ServerType::INITIATOR: {
   default:
    break;
   }

  } while (0);

 }

 void udp_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  auto sk = 0;
 }
 void udp_recv_cb(uv_udp_t* handle,
  ssize_t nread,
  const uv_buf_t* buf,
  const struct sockaddr* addr,
  unsigned flags) {
  auto sk = 0;
 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Server* __gpServer = nullptr;
}///namespace local 

