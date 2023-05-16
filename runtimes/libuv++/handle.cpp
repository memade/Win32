#include "stdafx.h"

namespace local {
 IHandle::IHandle(void* caller, void* route)
  : m_pCaller(caller)
  , m_pRoute(route) {

 }
 void IHandle::Caller(void* caller) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_pCaller = caller;
 }
 void IHandle::Route(void* route) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_pRoute = route;
 }
 void* IHandle::Route() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pRoute;
 }
 void* IHandle::Caller() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pCaller;
 }
 //////////////////////////////////////////////////////////////////////////////////////////////
 UvLoop::UvLoop(void* caller, void* route) : IHandle(caller, route) {
  Init();
 }
 UvLoop:: ~UvLoop() {
  UnInit();
 }
 void UvLoop::Init() {
  m_uv_loop = reinterpret_cast<uv_loop_t*>(malloc(sizeof uv_loop_t));
  if (m_uv_loop) {
   m_uv_loop->data = this;
   uv_loop_init(m_uv_loop);
  }
 }
 void UvLoop::UnInit() {
  SK_DELETE_PTR_C(m_uv_loop);
 }
 void* UvLoop::Handle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return reinterpret_cast<void*>(m_uv_loop);
 }
 void UvLoop::Release() const {
  delete this;
 }
 bool UvLoop::Activate() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return true;
 }
 void UvLoop::Close() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (!m_uv_loop)
   return;
  uv_walk(m_uv_loop,
   [](uv_handle_t* handle, void* arg) {
    if (!uv_is_closing(handle))
     uv_close(handle,
      [](uv_handle_t* handle) {

      });
   }, NULL);
  uv_run(m_uv_loop, UV_RUN_DEFAULT);
  uv_loop_close(m_uv_loop);
 }
 //////////////////////////////////////////////////////////////////////////////////////////////
 UvHandle::UvHandle(const HandleType& type, UvLoop* loop, void* caller, void* route)
  : m_pLoop(loop)
  , m_Type(type),
  IHandle(caller, route) {
  Init();
 }
 UvHandle:: ~UvHandle() {
  UnInit();
 }
 void UvHandle::Init() {
  bool success = false;
  switch (m_Type) {
  case HandleType::UV_TCP: {
   if (!m_pLoop)
    break;
   m_uv_handle = malloc(sizeof uv_tcp_t);
   if (!m_uv_handle)
    break;
   ((uv_tcp_t*)m_uv_handle)->data = this;
   if (0 != uv_tcp_init((uv_loop_t*)m_pLoop->Handle(), (uv_tcp_t*)m_uv_handle))
    break;
   success = true;
  }break;
  case HandleType::UV_UDP: {
   if (!m_pLoop)
    break;
   m_uv_handle = malloc(sizeof uv_udp_t);
   if (!m_uv_handle)
    break;
   ((uv_udp_t*)m_uv_handle)->data = this;
   if (0 != uv_udp_init((uv_loop_t*)m_pLoop->Handle(), (uv_udp_t*)m_uv_handle))
    break;
   success = true;
  }break;
  case HandleType::UV_PIPE_0: {
   if (!m_pLoop)
    break;
   m_uv_handle = malloc(sizeof uv_pipe_t);
   if (!m_uv_handle)
    break;
   ((uv_pipe_t*)m_uv_handle)->data = this;
   if (0 != uv_pipe_init((uv_loop_t*)m_pLoop->Handle(), (uv_pipe_t*)m_uv_handle, 0))
    break;
   success = true;
  }break;
  case HandleType::UV_PIPE_1: {
   if (!m_pLoop)
    break;
   m_uv_handle = malloc(sizeof uv_pipe_t);
   if (!m_uv_handle)
    break;
   ((uv_pipe_t*)m_uv_handle)->data = this;
   if (0 != uv_pipe_init((uv_loop_t*)m_pLoop->Handle(), (uv_pipe_t*)m_uv_handle, 1))
    break;
   success = true;
  }break;
  case HandleType::UV_ASYNC: {
   if (!m_pLoop)
    break;
   m_uv_handle = malloc(sizeof uv_async_t);
   if (!m_uv_handle)
    break;
   ((uv_async_t*)m_uv_handle)->data = this;
   if (0 != uv_async_init((uv_loop_t*)m_pLoop->Handle(), (uv_async_t*)m_uv_handle, Protocol::uv_async_cb))
    break;
   success = true;
  }break;
  default:
   break;
  }
 }
 void UvHandle::UnInit() {
  SK_DELETE_PTR_C(m_uv_handle);
 }
 UvLoop* UvHandle::Loop() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pLoop;
 }
 void* UvHandle::Handle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_uv_handle;
 }
 void UvHandle::Release() const {
  delete this;
 }
 bool UvHandle::Activate() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return true;
 }
 void UvHandle::UserData(void* udata) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_uv_handle)
   reinterpret_cast<uv_handle_t*>(m_uv_handle)->data = udata;
 }
 void* UvHandle::UserData() const {
  void* result = nullptr;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_uv_handle)
   result = reinterpret_cast<uv_handle_t*>(m_uv_handle)->data;
  return result;
 }
 void UvHandle::AsyncReqType(const AsyncType& async_type) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_AsyncReqType.store(async_type);
 }
 AsyncType UvHandle::AsyncReqType() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_AsyncReqType.load();
 }
 HandleType UvHandle::Type() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Type;
 }
 void UvHandle::Status(const HandleStatus& status) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Status.store(status);
 }
 HandleStatus UvHandle::Status() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load();
 }
 //////////////////////////////////////////////////////////////////////////////////////////////



































 Handle::Handle(const Type& handle_type, void* caller /*= nullptr*/, Handle* async /*= nullptr*/)
  : m_type(handle_type)
  , m_caller(caller)
  , m_async(async) {
  Init();
 }
 Handle::~Handle() {
  UnInit();
 }
 void Handle::Init(void* caller /*= nullptr*/) {
  switch (m_type) {
  case Type::UV_UDP_SEND:
   m_uv_handle = malloc(sizeof(uv_udp_send_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_udp_send_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_WRITE:
   m_uv_handle = malloc(sizeof(uv_write_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_write_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_LOOP:
   m_uv_handle = malloc(sizeof(uv_loop_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_loop_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_TCP:
   m_uv_handle = malloc(sizeof(uv_tcp_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_tcp_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_UDP:
   m_uv_handle = malloc(sizeof(uv_udp_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_udp_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_PIPE:
   m_uv_handle = malloc(sizeof(uv_pipe_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_pipe_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_ASYNC:
   m_uv_handle = malloc(sizeof(uv_async_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_async_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  case Type::UV_CONNECT:
   m_uv_handle = malloc(sizeof(uv_connect_t));
   if (!m_uv_handle)
    break;
   reinterpret_cast<uv_connect_t*>(m_uv_handle)->data = caller ? caller : this;
   break;
  default:
   break;
  }
 }
 void Handle::UnInit() {
  SK_DELETE_PTR_C(m_uv_handle);
  SK_DELETE_PTR_BUFFER(m_uv_buf.base);
 }
 void Handle::HandleUserData(void* uv_user_data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (uv_user_data)
   Init(uv_user_data);
 }
 void* Handle::HandleUserData() const {
  void* result = nullptr;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_uv_handle) {
   switch (m_type) {
   case Type::UV_UDP_SEND:
    result = reinterpret_cast<uv_udp_send_t*>(m_uv_handle)->data;
    break;
   case Type::UV_WRITE:
    result = reinterpret_cast<uv_write_t*>(m_uv_handle)->data;
    break;
   case Type::UV_LOOP:
    result = reinterpret_cast<uv_loop_t*>(m_uv_handle)->data;
    break;
   case Type::UV_TCP:
    result = reinterpret_cast<uv_tcp_t*>(m_uv_handle)->data;
    break;
   case Type::UV_UDP:
    result = reinterpret_cast<uv_udp_t*>(m_uv_handle)->data;
    break;
   case Type::UV_PIPE:
    result = reinterpret_cast<uv_pipe_t*>(m_uv_handle)->data;
    break;
   case Type::UV_ASYNC:
    result = reinterpret_cast<uv_async_t*>(m_uv_handle)->data;
    break;
   default:
    break;
   }
  }
  return result;
 }
 bool Handle::AsyncWriteRequest() {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_type != Type::UV_WRITE)
    break;
   if (!m_async)
    break;
   if (!m_uv_buf.base || m_uv_buf.len <= 0)
    break;
   m_async->AsyncTypeSet(AsyncType::TYPE_WRITE);
   m_async->Route(this);
   if (0 != uv_async_send(m_async->handle<uv_async_t>()))
    break;
   result = true;
  } while (0);
  return result;
 }
 void Handle::WriteCb(const Handle::tfUvWriteCb& write_cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_UvWirteCb = write_cb;
 }
 void Handle::WriteCb(uv_write_t* req, int status) const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_UvWirteCb)
   m_UvWirteCb(req, status);
 }
 const uv_buf_t& Handle::Buf() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_uv_buf;
 }
 void Handle::Buf(const char* data, const size_t& len) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!data || len <= 0)
    break;
   SK_DELETE_PTR_BUFFER(m_uv_buf.base);
   m_uv_buf.len = len;
   m_uv_buf.base = new char[m_uv_buf.len];
   if (!m_uv_buf.base)
    break;
   ::memset(m_uv_buf.base, 0x00, m_uv_buf.len);
   ::memcpy(m_uv_buf.base, data, m_uv_buf.len);
  } while (0);
 }
 const Handle::Type& Handle::TypeGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_type;
 }
 void Handle::AsyncTypeSet(const AsyncType& type) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_async_req_type = type;
 }
 const AsyncType& Handle::AsyncTypeGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_async_req_type;
 }
 void Handle::StatusSet(const HandleStatus& status) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_status = status;
 }
 const HandleStatus& Handle::StatusGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_status;
 }
 void Handle::Caller(void* caller) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_caller = caller;
 }
 void* Handle::Caller() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_caller;
 }
 const Handle* Handle::Async() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_async;
 }
 void Handle::Route(Handle* route) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_route = route;
 }
 Handle* Handle::Route() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_route;
 }
 bool Handle::Close() {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_type != Type::UV_LOOP) {
   if (!m_async) {
    if (!uv_is_closing((uv_handle_t*)m_uv_handle)) {
     m_status = HandleStatus::STATUS_CLOSING;
     uv_close((uv_handle_t*)m_uv_handle, NULL);
     m_status = HandleStatus::STATUS_CLOSED;
    }
    result = true;
   }
   else {
    do {
     if (m_status == HandleStatus::STATUS_CLOSING || m_status == HandleStatus::STATUS_CLOSED) {
      result = true;
      break;
     }
     m_status = HandleStatus::STATUS_CLOSING;
     m_async->m_route = this;
     m_async->m_async_req_type = AsyncType::TYPE_CLOSE_HANDLE;
     if (0 != uv_async_send((uv_async_t*)m_async->m_uv_handle))
      break;
     result = true;
    } while (0);
   }
  }

  do {
   if (m_type != Type::UV_LOOP || !m_uv_handle)
    break;
   uv_walk((uv_loop_t*)m_uv_handle,
    [](uv_handle_t* handle, void* arg) {
     if (!uv_is_closing(handle))
      uv_close(handle, NULL);
    }, NULL);
   uv_run((uv_loop_t*)m_uv_handle, UV_RUN_DEFAULT);
   uv_loop_close((uv_loop_t*)m_uv_handle);
  } while (0);

  return result;
 }
 bool Handle::AsyncClose(Handle* pAsync) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!pAsync)
    break;
   if (m_status == HandleStatus::STATUS_CLOSING || m_status == HandleStatus::STATUS_CLOSED) {
    result = true;
    break;
   }
   m_status = HandleStatus::STATUS_CLOSING;
   pAsync->m_route = this;
   pAsync->m_async_req_type = AsyncType::TYPE_CLOSE_HANDLE;
   if (0 != uv_async_send((uv_async_t*)pAsync->m_uv_handle))
    break;
#if 0
   std::atomic_bool closed = false;
   m_close_cb =
    [&]() {
    closed.store(true);
   };
   std::future<bool> fresult = std::async(
    [&]()->bool {
     do {
      if (closed.load())
       return true;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
     } while (1);
     return false;
    });
   result = fresult.get();
#endif
   result = true;
  } while (0);
  return result;



  return result;
 }




}///namespace local