#include "stdafx.h"

namespace local {
 Session::Session(const SessionType& type, Server* server, const struct sockaddr* psockaddr)
  : m_SessionType(type)
  , m_pServer(server) {
  if (psockaddr)
   memcpy(&m_SockAddr, psockaddr, sizeof(struct sockaddr));
  Init();
 }
 Session::~Session() {
  UnInit();
 }
 void Session::UnInit() {
  SK_DELETE_PTR(m_pReadStream);
  SK_DELETE_PTR(m_pWriteStream);
  SK_DELETE_PTR(m_pUvClient);
  SK_DELETE_PTR(m_pUvAsync);
  SK_DELETE_PTR(m_pUvLoop);
  SK_DELETE_PTR_C(m_pUvConnect);
 }
 void Session::Release() const {
  delete this;
 }
 void Session::Init() {
  m_Status.store(SessionStatus::Unready);

  m_pReadStream = new Stream(0xFFFF, StreamType::Read);
  m_pWriteStream = new Stream(0xFFFF, StreamType::Write);

  switch (m_SessionType) {
  case SessionType::IPC_SESSION_CLIENT: {
   m_pUvLoop = new UvLoop();
   m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pUvLoop->Handle(), this);
   m_pUvClient = new UvHandle(HandleType::UV_PIPE_1, m_pUvLoop->Handle(), this);
   m_pUvConnect = reinterpret_cast<uv_connect_t*>(malloc(sizeof uv_connect_t));
   if (!m_pUvConnect)
    break;
   m_pUvConnect->data = this;
   m_Status.store(SessionStatus::Ready);
  }break;
  case SessionType::IPC_SESSION_SERVER: {
   if (!m_pServer)
    break;
   m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pServer->LoopHandle()->Handle(), this);
   m_pUvClient = new UvHandle(HandleType::UV_PIPE_1, m_pServer->LoopHandle()->Handle(), this);
   if (0 != uv_accept(T_GET_HANDLE<uv_stream_t>(m_pServer->ServerHandle()), T_GET_HANDLE<uv_stream_t>(m_pUvClient)))
    break;
   m_Identify = Protocol::make_pipe_session_identify(m_SessionType);
   if (m_Identify <= 0)
    break;
   Welcome();
   m_Status.store(SessionStatus::Ready);
  }break;
  case SessionType::TCP_SESSION_CLIENT: {
   m_pUvLoop = new UvLoop();
   m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pUvLoop->Handle(), this);
   m_pUvClient = new UvHandle(HandleType::UV_TCP, m_pUvLoop->Handle(), this);
   m_pUvConnect = reinterpret_cast<uv_connect_t*>(malloc(sizeof uv_connect_t));
   if (!m_pUvConnect)
    break;
   m_pUvConnect->data = this;
   m_Status.store(SessionStatus::Ready);
  }break;
  case SessionType::TCP_SESSION_SERVER: {
   if (!m_pServer)
    break;
   m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pServer->LoopHandle()->Handle(), this);
   m_pUvClient = new UvHandle(HandleType::UV_TCP, m_pServer->LoopHandle()->Handle(), this);
   if (0 != uv_accept(T_GET_HANDLE<uv_stream_t>(m_pServer->ServerHandle()), T_GET_HANDLE<uv_stream_t>(m_pUvClient)))
    break;
   int len = sizeof(sockaddr);
   if (0 != uv_tcp_getpeername(T_GET_HANDLE<uv_tcp_t>(m_pUvClient), &m_SockAddr, &len))
    break;
   m_Identify = Protocol::make_sock_session_identify(m_SessionType, m_SockAddr);
   if (m_Identify <= 0)
    break;
   m_Address = Protocol::unmake_sock_session_identify(m_Identify);
   Welcome();
   m_Status.store(SessionStatus::Ready);
  }break;
  case SessionType::UDP_SESSION_CLIENT: {
   m_pUvLoop = new UvLoop();
   /*m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pUvLoop->Handle(), this);*/
   m_pUvClient = new UvHandle(HandleType::UV_UDP, m_pUvLoop->Handle(), this);
   m_Status.store(SessionStatus::Ready);
  }break;
  case SessionType::UDP_SESSION_SERVER: {
   if (!m_pServer)
    break;
   m_pUvAsync = new UvHandle(HandleType::UV_ASYNC, m_pServer->LoopHandle()->Handle(), this);
   if (!Protocol::parser_ipaddr((const sockaddr_storage*)&m_SockAddr, m_IPV4, m_Port))
    break;
   m_Identify = Protocol::make_sock_session_identify(SessionType::UDP_SESSION_SERVER, m_SockAddr);
   if (m_Identify <= 0)
    break;
   m_Address = Protocol::unmake_sock_session_identify(m_Identify);
   m_Status.store(SessionStatus::Ready);
  }break;
  default:
   break;
  }
 }

 const TypeIdentify& Session::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }
 void Session::Address(const std::string& address) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Address = address;
 }
 const std::string& Session::Address() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Address;
 }
 const std::string& Session::IP() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IPV4;
 }
 const struct sockaddr& Session::SockAddr() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_SockAddr;
 }
 const u_short& Session::Port() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Port;
 }
 bool Session::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load() == SessionStatus::Ready || m_Status.load() == SessionStatus::Started;
 }
 SessionStatus Session::Status() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load();
 }
 void Session::Status(const SessionStatus& status) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Status.store(status);
 }
 bool Session::Start(const std::string& address, const tfConnectCb& connect_cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_Status.load() != SessionStatus::Ready && m_Status.load() != SessionStatus::Opened)
   return false;
  switch (m_Status.load()) {
  case SessionStatus::Unready:
   return false;
  case SessionStatus::Starting:
  case SessionStatus::Started:
   return true;
  default:
   m_Status.store(SessionStatus::Starting);
   break;
  }

  switch (m_SessionType) {
  case SessionType::IPC_SESSION_CLIENT: {
   if (!m_pUvLoop)
    break;
   if (address.empty())
    break;
   m_ConnectCb = connect_cb;
   uv_pipe_connect(m_pUvConnect, T_GET_HANDLE<uv_pipe_t>(m_pUvClient), address.c_str(), ConnectCb);
   m_Threads.emplace_back([this]() {uv_run((uv_loop_t*)m_pUvLoop->Handle(), UV_RUN_DEFAULT); });
   m_Status.store(SessionStatus::Started);
  }break;
  case SessionType::IPC_SESSION_SERVER: {
   if (m_Identify <= 0)
    break;
   if (!m_pUvClient)
    break;
   if (0 != uv_read_start(T_GET_HANDLE<uv_stream_t>(m_pUvClient), AllocCb, ReadCb))
    break;
   m_Status.store(SessionStatus::Started);
  }break;
  case SessionType::TCP_SESSION_CLIENT: {
   if (!m_pUvLoop)
    break;
   if (address.empty())
    break;
   m_ConnectCb = connect_cb;
   struct sockaddr_in addr = { 0 };
   std::string ip;
   u_short port = 0;
   if (!Protocol::parser_ipaddr(address, ip, port))
    break;
   if (uv_ip4_addr(ip.c_str(), port, &addr) != 0)
    break;
   memcpy(&m_SockAddr, &addr, sizeof(struct sockaddr));
   m_Identify = Protocol::make_sock_session_identify(m_SessionType, m_SockAddr);
   if (Protocol::parser_ipaddr((const sockaddr_storage*)&m_SockAddr, m_IPV4, m_Port))
    m_Address = m_IPV4 + ":" + std::to_string(m_Port);
   if (0 != uv_tcp_connect(m_pUvConnect, T_GET_HANDLE<uv_tcp_t>(m_pUvClient), (const struct sockaddr*)&addr, ConnectCb))
    break;
   m_Threads.emplace_back([this]() {uv_run((uv_loop_t*)m_pUvLoop->Handle(), UV_RUN_DEFAULT); });
   m_Status.store(SessionStatus::Started);
  }break;
  case SessionType::TCP_SESSION_SERVER: {
   if (m_Identify <= 0)
    break;
   if (!m_pUvClient)
    break;
   if (0 != uv_read_start((uv_stream_t*)m_pUvClient->Handle(), AllocCb, ReadCb))
    break;
   m_Status.store(SessionStatus::Started);
  }break;
  case SessionType::UDP_SESSION_CLIENT: {
   if (!m_pUvLoop)
    break;
   if (address.empty())
    break;
   m_ConnectCb = connect_cb;
   struct sockaddr_in addr = { 0 };
   std::string ip;
   u_short port = 0;
   if (!Protocol::parser_ipaddr(address, ip, port))
    break;
   if (uv_ip4_addr(ip.c_str(), port, &addr) != 0)
    break;
   memcpy(&m_SockAddr, &addr, sizeof(struct sockaddr));
   m_Identify = Protocol::make_sock_session_identify(m_SessionType, m_SockAddr);
   if (Protocol::parser_ipaddr((const sockaddr_storage*)&m_SockAddr, m_IPV4, m_Port))
    m_Address = m_IPV4 + ":" + std::to_string(m_Port);
   if (0 != uv_udp_connect(T_GET_HANDLE<uv_udp_t>(m_pUvClient), &m_SockAddr))
    break;
   if (0 != uv_udp_recv_start(T_GET_HANDLE<uv_udp_t>(m_pUvClient), AllocCb, OnUdpReadCb))
    break;
   m_Threads.emplace_back([this]() {uv_run((uv_loop_t*)m_pUvLoop->Handle(), UV_RUN_DEFAULT); });
   m_Status.store(SessionStatus::Started);
   Hello();
   bool force_close = false;
   m_ConnectCb(true, force_close);
   if (force_close)
    m_Status.store(SessionStatus::ForceClose);
  }break;
  case SessionType::UDP_SESSION_SERVER: {
   if (m_Identify <= 0)
    break;
   m_Status.store(SessionStatus::Started);
  }break;
  default:
   break;
  }
  return m_Status.load() == SessionStatus::Started;
 }
 void Session::Stop() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  std::cout << __FUNCTION__ << std::endl;
  do {
   if (m_Status.load() == SessionStatus::Unready)
    break;
   m_Status.store(SessionStatus::Closing);
   switch (m_SessionType) {
   case SessionType::IPC_SESSION_CLIENT: {
    T_CLOSE_HANDLE(m_pUvClient, m_pUvAsync);
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
    m_pUvLoop->Close();
    Protocol::uv_close_default_loop();
    for (auto& it : m_Threads)
     it.join();
    m_Threads.clear();
   }break;
   case SessionType::IPC_SESSION_SERVER: {
    T_CLOSE_HANDLE(m_pUvClient, m_pUvAsync);
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
   }break;
   case SessionType::TCP_SESSION_CLIENT: {
    T_CLOSE_HANDLE(m_pUvClient, m_pUvAsync);
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
    m_pUvLoop->Close();
    Protocol::uv_close_default_loop();
    for (auto& it : m_Threads)
     it.join();
    m_Threads.clear();
   }break;
   case SessionType::TCP_SESSION_SERVER: {
    T_CLOSE_HANDLE(m_pUvClient, m_pUvAsync);
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
   }break;
   case SessionType::UDP_SESSION_CLIENT: {
#if 0
    if (!T_CLOSE_HANDLE(m_pUvClient, m_pUvAsync))
     ::exit(0);
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
#endif
    m_pUvClient->Close();
    m_pUvLoop->Close();
    Protocol::uv_close_default_loop();
    for (auto& it : m_Threads)
     it.join();
    m_Threads.clear();
   }break;
   case SessionType::UDP_SESSION_SERVER: {
    T_CLOSE_HANDLE(m_pUvAsync, m_pUvAsync);
   }break;
   default:
    break;
   }

  } while (0);

  m_Status.store(SessionStatus::Closed);
 }
 void Session::ForceClose() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  switch (m_Status) {
  case  SessionStatus::Closing:
   [[fallthrough]];
  case SessionStatus::Closed:
   break;
  case SessionStatus::Unknown:
   m_Status = (SessionStatus::Closed);
   break;
  default:
   m_Status = (SessionStatus::ForceClose);
   break;
  }
 }
 bool Session::Read(const char* data, const size_t& len) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (data && len > 0)
   result = *m_pReadStream << std::string(data, len);
  return result;
 }
 std::string Session::Read() {
  std::string result;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_pReadStream)
    break;
   if (!((*m_pReadStream) >> result))
    break;
  } while (0);
  return result;
 }
 bool Session::Write() {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_pWriteStream->Empty()) {
    result = true;
    break;
   }
   std::string send_pak;
   if (!(*m_pWriteStream >> send_pak))
    break;

   switch (m_SessionType) {
   case SessionType::TCP_SESSION_CLIENT:
    [[fallthrough]];
   case SessionType::TCP_SESSION_SERVER:
    [[fallthrough]];
   case SessionType::IPC_SESSION_CLIENT:
    [[fallthrough]];
   case SessionType::IPC_SESSION_SERVER: {
    UvWirte* req = new UvWirte(
     [&](void* route, int status) {
      if (status != 0) {
       auto session = reinterpret_cast<Session*>(route);
       if (session)
        session->ForceClose();
      }
     }, send_pak, this);
    *req << m_pUvClient->Handle();
    m_pUvAsync->AsyncReqType(AsyncType::TYPE_WRITE);
    m_pUvAsync->Route(req);
   }break;
   case SessionType::UDP_SESSION_SERVER: {
    if (!m_pServer || !m_pServer->ServerHandle())
     break;
    UvWirte* req = new UvWirte(
     [&](void* route, int status) {
      if (status != 0) {
       auto session = reinterpret_cast<Session*>(route);
       if (session)
        session->ForceClose();
      }
     }, send_pak, this);
    *req << m_pServer->ServerHandle()->Handle();
    *req << m_SockAddr;
    m_pUvAsync->AsyncReqType(AsyncType::TYPE_WRITE_UDP);
    m_pUvAsync->Route(req);
   }break;
   case SessionType::UDP_SESSION_CLIENT: {
    m_pUvLoop->Async(
     [this, send_pak](const uv_async_t* async) {
      do {
       UvWirte* write_req = new UvWirte(
        [&](void* route, int status) {
         if (status != 0) {
          auto session = reinterpret_cast<Session*>(route);
          if (session)
           session->ForceClose();
         }
        }, send_pak, this);
       *write_req << m_pUvClient->Handle();
       if (!write_req)
        break;
       int status = uv_udp_send(
        &write_req->write_udp,
        (uv_udp_t*)write_req->handle,
        &write_req->buf,
        1,
        write_req->addr.sa_family ? &write_req->addr : NULL,
        [](uv_udp_send_t* req, int status) {
         UvWirte* write_req = reinterpret_cast<UvWirte*>(req->data);
         if (write_req && write_req->write_cb)
          write_req->write_cb(write_req->route, status);
         SK_DELETE_PTR(write_req);
        });
       if (status == 0)
        break;
       if (write_req && write_req->write_cb)
        write_req->write_cb(write_req->route, status);
       SK_DELETE_PTR(write_req);
      } while (0);
     
     });
    /*m_pUvAsync->AsyncReqType(AsyncType::TYPE_WRITE_UDP);
    m_pUvAsync->Route(req);*/
   }break;
   default:
    break;
   }
   if (m_pUvAsync && 0 != uv_async_send(T_GET_HANDLE<uv_async_t>(m_pUvAsync)))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Session::Hello() {//!@ Private
  return *m_pWriteStream << Protocol::MakeStream(HEAD(CommandType::Hello), "Hello !");
 }
 bool Session::Welcome() {//!@ Private
  return *m_pWriteStream << Protocol::MakeStream(HEAD(CommandType::Welcome), "Welcome !");
 }
 bool Session::Write(const CommandType& cmd, const std::string& original_data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return *m_pWriteStream << Protocol::MakeStream(HEAD(CommandType(cmd)), original_data);
 }
 UvHandle* Session::ClientHandle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pUvClient;
 }
 void Session::AllocCb(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
  Session* pSession = T_GET_HANDLE_CALLER<Session>(client);
  pSession->m_pReadStream->Resize(suggested_size);
  buf->base = const_cast<char*>(pSession->m_pReadStream->Base());
  buf->len = pSession->m_pReadStream->BufferSize();
 }
 void Session::ReadCb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
  Session* pSession = T_GET_HANDLE_CALLER<Session>(client);
  if (nread < 0) {
   switch (nread) {
   case UV__EOF: break;
   case UV__ECONNRESET: break;
   case UV__ENOTSOCK: break;
   case UV__ECONNABORTED: break;
   default: break;
   }
   pSession->ForceClose();
  }
  else if (nread > 0) {
   if (!(*pSession->m_pReadStream << std::string(buf->base, nread))) {
    pSession->ForceClose();
   }
  }
  else {//!@ nread==0 ~

  }
 }
 void Session::ConnectCb(const bool& bConnect, bool& bForceClose) const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_ConnectCb)
   m_ConnectCb(bConnect, bForceClose);
 }
 void Session::ConnectCb(uv_connect_t* req, int status) {
  Session* pSession = reinterpret_cast<Session*>(req->data);
  if (!pSession)
   return;
  bool success = false;
  do {
   if (status != 0) {


    break;
   }

   switch (pSession->Type()) {
   case SessionType::TCP_SESSION_CLIENT: {
    if (0 != uv_read_start(T_GET_HANDLE<uv_stream_t>(pSession->ClientHandle()), AllocCb, ReadCb))
     break;
    success = true;
   }break;
   case SessionType::IPC_SESSION_CLIENT: {
    if (0 != uv_read_start(T_GET_HANDLE<uv_stream_t>(pSession->ClientHandle()), AllocCb, ReadCb))
     break;
    success = true;
   }break;
   default:
    break;
   }

  } while (0);


  bool force_close = false;
  pSession->ConnectCb(success, force_close);
  if (force_close)
   pSession->ForceClose();
 }
 void Session::OnUdpReadCb(uv_udp_t* client, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
  Session* pSession = T_GET_HANDLE_CALLER<Session>(client);
  do {
   if (!pSession)
    break;
   if (!addr || nread <= 0)
    break;
   if (!pSession->Read(buf->base, nread))
    pSession->ForceClose();
  } while (0);

  if (nread < 0 && pSession) {
   pSession->ForceClose();
  }
 }
 const SessionType& Session::Type() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_SessionType;
 }
 void Session::KeepAliveUpdate(const std::uint64_t& current) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_TimeKeepAlive.store(current);
 }
 bool Session::KeepAliveCheck(const std::uint64_t& current) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_TimeKeepAlive.load() == 0)
   m_TimeKeepAlive.store(current);
  return current - m_TimeKeepAlive.load() <= TIMEOUT_HEARBEAT_MS;
 }
 bool Session::Connect(const std::string& address, const tfConnectCb& connect_cb) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_Status == SessionStatus::Connected || \
    m_Status == SessionStatus::Connecting) {
    result = true;
    break;
   }
   m_Status = (SessionStatus::Connecting);
   m_ConnectCb = connect_cb;

   result = true;
  } while (0);

  return result;
 }




}///namespace local
