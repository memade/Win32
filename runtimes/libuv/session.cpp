#include "stdafx.h"

namespace local {

 Session::Session(const EnSessionType& session_type) : m_SessionType(session_type) {
  Init();
 }

 Session::~Session() {
  UnInit();
 }
 void Session::Release() const {
  delete this;
 }

 void Session::Init() {
  int r = -1;
  m_Status.store(SessionStatus::Opening);
  m_pReadStream = new Stream(0xFFFF, StreamType::Read);
  m_pWriteStream = new Stream(0xFFFF, StreamType::Write);
  uv_client_ = reinterpret_cast<decltype(uv_client_)>(new uv_tcp_t);
  uv_async_t_ = reinterpret_cast<decltype(uv_async_t_)>(new uv_async_t);
  uv_client_->data = this;

  switch (m_SessionType) {
  case EnSessionType::TCP_SESSION_CLIENT: {
   if (!uv_loop_t_)
    uv_loop_t_ = new uv_loop_t;
   if (r = uv_loop_init(uv_loop_t_) != 0)
    break;
   if (r = uv_tcp_init(uv_loop_t_, (uv_tcp_t*)uv_client_) != 0) {
    break;
   }
   if (r = uv_async_init(uv_loop_t_, uv_async_t_, AsyncCb) != 0) {
    ILibuv::uv_close_stream(uv_client_);
    break;
   }
   uv_thread_main_ = new uv_thread_t;
   uv_connect_t_ = new uv_connect_t;
   uv_connect_t_->data = this;
  }break;
  case EnSessionType::TCP_SESSION_SERVER: {
   if (!uv_server_)
    break;
   if (r = uv_tcp_init(uv_server_->loop, (uv_tcp_t*)uv_client_) != 0) {
    break;
   }
   if (r = uv_async_init(uv_server_->loop, uv_async_t_, AsyncCb) != 0) {
    ILibuv::uv_close_stream(uv_client_);
    break;
   }
   if (r = uv_accept(uv_server_, uv_client_) != 0) {
    ILibuv::uv_close_stream(uv_client_);
    break;
   }
   sockaddr_storage addr = { 0 };
   int len = sizeof(sockaddr_storage);
   if (r = uv_tcp_getpeername((const uv_tcp_t*)uv_client_, (struct sockaddr*)&addr, &len) != 0)
    break;
   ILibuv::uv_get_ipaddr(&addr, ip_, port_);
   if (r = uv_read_start(uv_client_, AllocCb, ReadCb) != 0) {
    ForceClose();
    break;
   }
  }break;
  case EnSessionType::PIPE_SESSION_CLIENT: {

  }break;
  case EnSessionType::PIPE_SESSION_SERVER: {

  }break;
  case EnSessionType::UDP_SESSION_CLIENT: {

  }break;
  case EnSessionType::UDP_SESSION_SERVER: {

  }break;
  default:
   break;
  }

  m_Status.store(r == 0 ? SessionStatus::Ready : SessionStatus::Unready);
 }
 void Session::UnInit() {
  SK_DELETE_PTR(uv_loop_t_);
  SK_DELETE_PTR(uv_async_t_);
  SK_DELETE_PTR(uv_client_);
  SK_DELETE_PTR(uv_thread_main_);
  SK_DELETE_PTR(uv_connect_t_);
  SK_DELETE_PTR(m_pReadStream);
  SK_DELETE_PTR(m_pWriteStream);
 }
 void Session::SetServerStream(uv_stream_t* server) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  uv_server_ = server;
 }
 const std::string& Session::IP() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return ip_;
 }
 const u_short& Session::Port() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return port_;
 }
 const u_short& Session::Key() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return port_;
 }
 bool Session::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load() == SessionStatus::Ready;
 }
 SessionStatus Session::Status() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load();
 }
 void Session::Status(const SessionStatus& status) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Status.store(status);
 }
 void Session::ForceClose() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_Status.load() == SessionStatus::Closing ||
    m_Status.load() == SessionStatus::Closed)
    break;
   m_Status.store(SessionStatus::Closing);
   async_req_t* req = new async_req_t;
   req->host = this;
   req->req_type = async_req_type::Close;
   uv_async_t_->data = req;
   uv_async_send(uv_async_t_);
  } while (0);
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
   write_req_t* write_req = new write_req_t;
   write_req->buf.len = static_cast<decltype(write_req->buf.len)>(send_pak.size());
   write_req->buf.base = new char[write_req->buf.len];
   write_req->handle = uv_client_;
   memcpy(write_req->buf.base, send_pak.data(), write_req->buf.len);
   async_req_t* async_req = new async_req_t;
   async_req->data = write_req;
   async_req->host = this;
   async_req->req_type = async_req_type::Write;
   uv_async_t_->data = async_req;
   if (0 != uv_async_send(uv_async_t_))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Session::Write(const CommandType& cmd, const std::string& original_data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return *m_pWriteStream << Protocol::MakeStream(HEAD(CommandType::Welcome), original_data);
 }

 void Session::AllocCb(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
  auto pSession = reinterpret_cast<Session*>(client->data);
  pSession->m_pReadStream->Resize(suggested_size);
  buf->base = const_cast<char*>(pSession->m_pReadStream->Base());
  buf->len = pSession->m_pReadStream->BufferSize();
 }
 void Session::ReadCb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
  auto pSession = reinterpret_cast<Session*>(client->data);
  if (nread < 0) {
   switch (nread) {
   case UV__EOF: {}break;
   case UV__ECONNRESET: {}break;
   default:break;
   }
   pSession->ForceClose();
  }
  else if (nread > 0) {
   if (!(*pSession->m_pReadStream << std::string(buf->base, nread))) {

    auto read_error = 0;
   }
  }
  else {//!@ nread==0 ~

  }
 }
 void Session::AsyncCb(uv_async_t* async_handle) {
  async_req_t* async = (async_req_t*)async_handle->data;
  Session* pSession = (Session*)async->host;
  switch (async->req_type) {
  case async_req_type::Write: {
   write_req_t* req = (write_req_t*)async->data;
   int write_ret = uv_write((uv_write_t*)req, (uv_stream_t*)req->handle, &req->buf, 1,
    [](uv_write_t* req, int status) {
     write_req_t* request = (write_req_t*)req;
     SK_DELETE_PTR(request->buf.base);
     SK_DELETE_PTR(request);
    });
   if (!write_ret)
    break;
   SK_DELETE_PTR(req->buf.base);
   SK_DELETE_PTR(req);
   pSession->ForceClose();
  }break;
  case async_req_type::Close: {
   uv_close((uv_handle_t*)async_handle,
    [](uv_handle_t* handle) {
     auto sk = 0;
    });
   uv_close((uv_handle_t*)pSession->uv_client_,
    [](uv_handle_t* handle) {
     Session* pSession = (Session*)handle->data;
     pSession->m_Status.store(SessionStatus::Closed);
    });
  }break;
  default:
   break;
  }
  SK_DELETE_PTR(async);
 }
 void Session::ConnectCb(uv_connect_t* req, int status) {
  Session* pSession = reinterpret_cast<Session*>(req->data);
  if (pSession) {
   if (status == 0) {
    status = uv_read_start(pSession->uv_client_, AllocCb, ReadCb);
   }
   pSession->Status(status == 0 ? ConnectionStatus::Connected : ConnectionStatus::Disconnected);
   if (pSession->m_ConnectCb) {
    connect_callback_route_t* connect_route = new connect_callback_route_t;
    connect_route->caller = pSession;
    connect_route->status = status;
    connect_route->route = nullptr;
    pSession->m_ConnectCb(connect_route);
    if (connect_route->connect_action >= (int)SessionStatus::Begin && \
     connect_route->connect_action <= (int)SessionStatus::End)
     pSession->Status(SessionStatus(connect_route->connect_action));
    SK_DELETE_PTR(connect_route);
   }
  }
 }

 const EnSessionType& Session::SessionType() const {
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
 void Session::Destory() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!uv_loop_t_)
    break;
   //if (m_Status.load() != SessionStatus::Closed)
   // break;
   ILibuv::uv_close_loop(uv_loop_t_);
   uv_thread_join(uv_thread_main_);
  } while (0);
 }
 bool Session::Connect(const std::string& ip, const u_short& port, const tfConnectCb& connect_cb) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_Status.load() == SessionStatus::Connected || \
    m_Status.load() == SessionStatus::Connecting) {
    result = true;
    break;
   }
   m_Status.store(SessionStatus::Connecting);
   m_ConnectCb = connect_cb;
   struct sockaddr_in addr = { 0 };
   if (uv_ip4_addr(ip.c_str(), port, &addr) != 0)
    break;
   if (uv_tcp_connect(uv_connect_t_, (uv_tcp_t*)uv_client_, (const struct sockaddr*)&addr, ConnectCb) != 0)
    break;
   if (0 != uv_thread_create(uv_thread_main_,
    [](void* arg) {
     Session* pSession = (Session*)arg;
     uv_run(pSession->uv_loop_t_, UV_RUN_DEFAULT);
    }, this))
    break;
    result = true;
  } while (0);
  return result;
 }
}///namespace local
