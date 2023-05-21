#include "stdafx.h"

namespace local {
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void uv_main(void* arg);
 void connection_cb(uv_stream_t* server, int status);
 void walk_process_cb(uv_handle_t* handle, void* arg);
 void udp_alloc_cb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);
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
   if (0 != uv_thread_create(&thread_main_, uv_main, this))
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
 void Server::RegisterOnAcceptCb(const tfOnAcceptCb&) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
 }
 void Server::RegisterOnReceiveReply(const tfOnReceiveReply&) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
 }
 void Server::RegisterOnSessionAppendBeforeCb(const tfOnSessionAppendBeforeCb&) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
 }
 void Server::RegisterOnSessionRemoveBeforeCb(const tfOnSessionRemoveBeforeCb&) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
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
 static void uv_main(void* arg) {
  Server* pServer = nullptr;
  uv_loop_t* loop = nullptr;
  uv_handle_t* server = nullptr;
  do {
   pServer = reinterpret_cast<Server*>(arg);
   if (!pServer)
    break;

   loop = new uv_loop_t;
   loop->data = new UserData;
   USERDATA_PTR(loop)->Caller(arg);
   if (!loop)
    break;
   if (0 != uv_loop_init(loop))
    break;

   switch (pServer->ConfigGet()->Session()) {
   case SessionType::TCP: {
    server = (uv_handle_t*)new uv_tcp_t;
    if (!server)
     break;
    if (0 != uv_tcp_init(loop, (uv_tcp_t*)server))
     break;
    std::string ip;
    unsigned short port = 0;
    if (!Protocol::parser_ipaddr(pServer->ConfigGet()->Address(), ip, port))
     break;
    struct sockaddr_in addr = { 0 };
    if (0 != uv_ip4_addr(ip.c_str(), port, &addr))
     break;
    if (0 != uv_tcp_bind((uv_tcp_t*)server, (const struct sockaddr*)&addr, 0))
     break;
    if (0 != uv_listen((uv_stream_t*)server, SOMAXCONN, connection_cb))
     break;
    pServer->Status(ServerStatus::READY);
   }break;
   case SessionType::IPC: {
    server = (uv_handle_t*)new uv_pipe_t;
    if (!server)
     break;
    if (0 != uv_pipe_init(loop, (uv_pipe_t*)server, 0))
     break;
    if (0 != uv_pipe_bind((uv_pipe_t*)server, pServer->ConfigGet()->Address().c_str()))
     break;
    if (0 != uv_listen((uv_stream_t*)server, SOMAXCONN, connection_cb))
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
    if (!Protocol::parser_ipaddr(pServer->ConfigGet()->Address(), ip, port))
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

   if (pServer->Status() != ServerStatus::READY)
    break;
   server->data = new UserData;
   USERDATA_PTR(server)->Caller(arg);
   USERDATA_PTR(server)->Handle(server);
   USERDATA_PTR(server)->ServerTypeSet(ServerType::ACCEPTOR);

   do {
    uv_walk(loop, walk_process_cb, arg);
    uv_run(loop, uv_run_mode::UV_RUN_NOWAIT);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (pServer->IsClose()) {
     uv_close(server, Protocol::uv_close_cb);
     uv_walk(loop, Protocol::walk_close_handle_cb, arg);
     uv_run(loop, uv_run_mode::UV_RUN_DEFAULT);
     uv_loop_close(loop);
     break;
    }
   } while (1);

  } while (0);

  if (loop) {
   auto udata = USERDATA_PTR(loop);
   SK_DELETE_PTR(udata);
   SK_DELETE_PTR(loop);
  }

 }


 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void connection_cb(uv_stream_t* server, int status) {
  bool success = false;
  uv_handle_t* client = nullptr;
  do {
   if (status != 0 || !server)
    break;
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
   }break;
   default:
    break;
   }

   if (!success)
    break;
   client->data = new Session;
   SESSION_PTR(client)->Caller(SESSION_PTR(server)->Caller());
   SESSION_PTR(client)->Handle(client);
   SESSION_PTR(client)->ServerTypeSet(ServerType::INITIATOR);
   SESSION_PTR(client)->Address(client_address);
   LOG_OUTPUT(std::format("Session logon on({})", client_address));
   SESSION_PTR(client)->Write(CommandType::WELCOME, "Welcome to server.");

  } while (0);


  do {
   if (success)
    break;
   if (!client)
    break;
   if (!uv_is_closing(client)) {
    uv_close(client, Protocol::uv_close_cb);
    break;
   }

   UserData* udata = USERDATA_PTR(client);
   SK_DELETE_PTR(udata);
   SK_DELETE_PTR(client);
  } while (0);
 }

 void walk_process_cb(uv_handle_t* handle, void* arg) {
  if (uv_is_closing(handle))
   return;
  Server* pServer = reinterpret_cast<Server*>(arg);
  Session* pSession = SESSION_PTR(handle);
  bool client_force_close = false;
  const unsigned long long current_time_ms = shared::TimeStampUTC<std::chrono::milliseconds>();

  do {
   if (!pSession || !pServer)
    break;

   switch (pSession->ServerTypeGet()) {
   case ServerType::UNKNOWN: {

   }break;
   case ServerType::ACCEPTOR: {

   }break;
   case ServerType::INITIATOR: {
    do {
     if (pSession->ActivationTime(current_time_ms) >= pServer->ConfigGet()->SessionTimeoutMS()) {
      client_force_close = true;
      break;
     }

     do {//!@ write
      write_req_t* req = pSession->Write();
      if (!req)
       break;
      req->handle = reinterpret_cast<decltype(req->handle)>(handle);
      if (0 != uv_write(&req->write, reinterpret_cast<uv_stream_t*>(req->handle), &req->buf, 1, Protocol::uv_write_cb)) {
       SK_DELETE_PTR(req);
       client_force_close = true;
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
       pSession->Status(SessionStatus::FORCECLOSE);
       break;
      }
      switch (head.Command()) {
      case CommandType::HELLO: {
       pSession->Write(CommandType::WELCOME, "Welcome!");
      }break;
      default:
       break;
      }
      LOG_OUTPUT(std::format("recv message({:X}:{})", static_cast<unsigned long>(head.Command()), message));
     } while (0);

     client_force_close = pSession->Status() == SessionStatus::FORCECLOSE;
    } while (0);


   }break;
   default:
    break;
   }

  } while (0);


  if (client_force_close) {
   uv_close(handle, Protocol::uv_close_cb);
  }
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

