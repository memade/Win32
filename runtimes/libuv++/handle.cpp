#include "stdafx.h"

namespace local {
 //////////////////////////////////////////////////////////////////////////////////////////////
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
 UvLoop::UvLoop() {
  Init();
 }
 UvLoop::~UvLoop() {
  UnInit();
 }
 void UvLoop::Init() {
  loop_ = new uv_loop_t;
  async_ = new uv_async_t;
  loop_->data = this;
  async_->data = this;
  uv_loop_init(loop_);
  uv_async_init(loop_, async_,
   [](uv_async_t* async) {
    auto loop = reinterpret_cast<UvLoop*>(async->data);
    loop->async_cb_(async);
   });
 }
 void UvLoop::UnInit() {
  SK_DELETE_PTR(async_);
  SK_DELETE_PTR(loop_);
 }
 uv_loop_t* UvLoop::Handle() const {
  return loop_;
 }
 int UvLoop::Run() const {
  return uv_run(loop_, UV_RUN_DEFAULT);
 }
 bool UvLoop::Async(const tfAsyncCb& async_cb) {
  bool result = false;
  do {
   async_cb_ = async_cb;
   if (0 != uv_async_send(async_)) {
    std::cout << "uv_async_send error." << std::endl;
    break;
   }
   result = true;
  } while (0);
  return result;
 }

 /*
* 在libuv中，uv_run函数是用于启动事件循环的函数。它会一直运行，直到事件循环中没有待处理的事件或者显式地停止事件循环。
* uv_run函数的返回值表示事件循环的退出状态。具体来说，返回值为1表示事件循环正常退出并且没有待处理的事件，返回值为0表示事件循环被显式停止，返回值为负数表示事件循环出现错误。
* 如果uv_run返回1，这意味着事件循环已经处理完当前的事件队列，并且没有其他待处理的事件。这通常发生在没有更多的事件被添加到事件队列或者所有的事件都被处理完之后。
* 这种情况下，你可以根据需要决定是否继续运行事件循环或者退出程序。
*/
 void UvLoop::Close() {
 /* std::lock_guard<std::mutex> lock{*m_Mutex};*/
  do {
   if (!loop_)
    break;
   std::vector<UvHandle*> handles;
   uv_walk(loop_,
    [](uv_handle_t* handle, void* arg) {
     auto handles = reinterpret_cast<std::vector<UvHandle*>*>(arg);
     if (handle->type != UV_ASYNC) {
      auto h = reinterpret_cast<UvHandle*>(handle->data);
      handles->emplace_back(h);
     }
    }, &handles);

   do {
    if (handles.empty())
     break;
    for (auto it = handles.begin(); it != handles.end();) {
     auto status = (*it)->Status();
     if (status > HandleStatus::STATUS_CLOSED) {
      ++it;
      continue;
     }
     it = handles.erase(it);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
   } while (1);

   async_cb_ = [](const uv_async_t* async) {
    if (!uv_is_closing((uv_handle_t*)async)) {
     uv_close((uv_handle_t*)async,
      [](uv_handle_t* handle) {
       handle->data = nullptr;
      });
    }
   };
   uv_async_send(async_);
   do {
    if (async_->data == nullptr)
     break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
   } while (1);
   uv_run(loop_, UV_RUN_DEFAULT);
   uv_loop_close(loop_);
  } while (0);
 }
 //////////////////////////////////////////////////////////////////////////////////////////////
 UvHandle::UvHandle(const HandleType& type, uv_loop_t* loop, void* caller, void* route)
  : loop_(loop)
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
   if (!loop_)
    break;
   handle_ = reinterpret_cast<decltype(handle_)>(new uv_tcp_t);
   if (!handle_)
    break;
   handle_->data = this;
   if (0 != uv_tcp_init(loop_, (uv_tcp_t*)handle_))
    break;
   success = true;
  }break;
  case HandleType::UV_UDP: {
   if (!loop_)
    break;
   handle_ = reinterpret_cast<decltype(handle_)>(new uv_udp_t);
   if (!handle_)
    break;
   handle_->data = this;
   if (0 != uv_udp_init(loop_, (uv_udp_t*)handle_))
    break;
   success = true;
  }break;
  case HandleType::UV_PIPE_0: {
   if (!loop_)
    break;
   handle_ = reinterpret_cast<decltype(handle_)>(new uv_pipe_t);
   if (!handle_)
    break;
   handle_->data = this;
   if (0 != uv_pipe_init(loop_, (uv_pipe_t*)handle_, 0))
    break;
   success = true;
  }break;
  case HandleType::UV_PIPE_1: {
   if (!loop_)
    break;
   handle_ = reinterpret_cast<decltype(handle_)>(new uv_pipe_t);
   if (!handle_)
    break;
   handle_->data = this;
   if (0 != uv_pipe_init(loop_, (uv_pipe_t*)handle_, 1))
    break;
   success = true;
  }break;
  case HandleType::UV_ASYNC: {
   if (!loop_)
    break;
   handle_ = reinterpret_cast<decltype(handle_)>(new uv_async_t);
   if (!handle_)
    break;
   handle_->data = this;
   if (0 != uv_async_init(loop_, (uv_async_t*)handle_, Protocol::uv_async_cb))
    break;
   success = true;
  }break;
  default:
   break;
  }
  if (success)
   m_Status.store(HandleStatus::STATUS_READY);
 }
 void UvHandle::UnInit() {
  SK_DELETE_PTR(handle_);
 }
 uv_loop_t* UvHandle::Loop() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return loop_;
 }
 uv_handle_t* UvHandle::Handle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return handle_;
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
  if (handle_)
   reinterpret_cast<uv_handle_t*>(handle_)->data = udata;
 }
 void* UvHandle::UserData() const {
  void* result = nullptr;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (handle_)
   result = reinterpret_cast<uv_handle_t*>(handle_)->data;
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
 void UvHandle::Close() {
  std::cout << "On close handle 0" << std::endl;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  std::cout << "On close handle 1" << std::endl;
  do {
   if (!handle_)
    break;
   if (!handle_->loop || !handle_->loop->data)
    break;
   std::cout << "On close handle 2" << std::endl;
   if (m_Status.load() <= HandleStatus::STATUS_CLOSED) {
    m_Status.store(HandleStatus::STATUS_CLOSED);
    break;
   }
   m_Status.store(HandleStatus::STATUS_CLOSING);
   std::cout << "On close handle 3" << std::endl;
   auto loop = reinterpret_cast<UvLoop*>(handle_->loop->data);
   loop->Async(
    [this](const uv_async_t* async) {
     std::cout << "On close handle 4" << std::endl;
     uv_close(handle_,
     [](uv_handle_t* handle) {
       UvHandle* pHandle = reinterpret_cast<UvHandle*>(handle->data);
       pHandle->Status(HandleStatus::STATUS_CLOSED);
      });
    });
  } while (0);
 }



}///namespace local