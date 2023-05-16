#if !defined(__4A69B522_AFFA_401E_AF73_A131565C1555__)
#define __4A69B522_AFFA_401E_AF73_A131565C1555__

namespace local {

#if defined(_WIN32)
#define notify_parent_process() ((void) 0)
#else
 extern void notify_parent_process(void);
#endif

 enum class AsyncType : unsigned long {
  TYPE_UNKNOWN = 0x00,
  TYPE_WRITE = 0x10,
  TYPE_WRITE_UDP = 0x20,
  TYPE_WRITE_UDP_SERVER_SESSION = 0x21,
  TYPE_WRITE_UDP_CLIENT_SESSION = 0x22,
  TYPE_CLOSE_HANDLE = 0x30,
  TYPE_CLOSE_ASYNC_HANDLE = 0x40,
 };
 enum class HandleStatus : unsigned long {
  STATUS_UNKNOWN = 0,
  STATUS_CLOSING = 1,
  STATUS_CLOSED = 2,
  STATUS_RUNNING = 3,
  STATUS_ERROR = 4,
 };

 enum class HandleType : unsigned long {
  UV_UNKNOWN_HANDLE = 0,
#define XX(uc, lc) UV_##uc,
  UV_HANDLE_TYPE_MAP(XX)
#undef XX
  UV_FILE,
  UV_LOOP,
  UV_CONNECT,
  UV_THREAD,
  UV_WRITE,
  UV_PIPE_0,
  UV_PIPE_1,
  UV_UDP_SEND,
  UV_HANDLE_TYPE_MAX
 };

 class UvWirte final {
  using tfUvWriteCb = std::function<void(void*, int status)>;
 public:
  UvWirte(const tfUvWriteCb& in_write_cb = nullptr, const std::string& data = "", void* in_route = nullptr)
   : route(in_route)
   , write_cb(in_write_cb) {
   *this << data;
   write.data = this;
   write_udp.data = this;
  }
  ~UvWirte() {
   SK_DELETE_PTR_BUFFER(buf.base);
   buf.len = 0;
  }
  void operator<<(const std::string& data) {
   if (!data.empty()) {
    buf.len = data.size();
    buf.base = new char[buf.len];
    memcpy(buf.base, data.data(), buf.len);
   }
  }
  void operator<<(const struct sockaddr& in_addr) {
   memcpy(&addr, &in_addr, sizeof(addr));
  }
  void operator<<(void* h) {
   handle = h;
  }
  void operator<<(const tfUvWriteCb& cb) {
   write_cb = cb;
  }
 public:
  uv_write_t write = { 0 };
  uv_udp_send_t write_udp = { 0 };
  uv_buf_t buf = { 0 };
  struct sockaddr addr = { 0 };
  void* handle = nullptr;
  tfUvWriteCb write_cb = nullptr;
  void* route = nullptr;
 };

 class IHandle {
 protected:
  IHandle(void* caller, void* route);
  virtual void Init() = 0;
  virtual void UnInit() = 0;
 public:
  virtual void* Handle() const = 0;
  virtual void Release() const = 0;
  virtual bool Activate() const = 0;
  virtual void Route(void*);
  virtual void Caller(void*);
  virtual void* Route() const;
  virtual void* Caller() const;
 protected:
  void* m_pRoute = nullptr;
  void* m_pCaller = nullptr;
  std::atomic_bool m_IsOpen = false;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

 class UvLoop final : public IHandle {
 public:
  UvLoop(void* caller = nullptr, void* route = nullptr);
  virtual ~UvLoop();
 protected:
  void Init() override final;
  void UnInit() override final;
 public:
  void* Handle() const override final;
  void Release() const override final;
  bool Activate() const override final;
  void Close();
 private:
  uv_loop_t* m_uv_loop = nullptr;
 };

 class UvHandle final : public IHandle {
 public:
  UvHandle(const HandleType&, UvLoop* loop = nullptr, void* caller = nullptr, void* route = nullptr);
  virtual ~UvHandle();
 protected:
  void Init() override final;
  void UnInit() override final;
 public:
  void* Handle() const override final;
  UvLoop* Loop() const;
  void Release() const override final;
  bool Activate() const override final;
  void AsyncReqType(const AsyncType&);
  AsyncType AsyncReqType() const;
  HandleType Type() const;
  void Status(const HandleStatus&);
  HandleStatus Status() const;
  void UserData(void*);
  void* UserData() const;
 private:
  void* m_uv_handle = nullptr;
  UvLoop* m_pLoop = nullptr;
  UvHandle* m_pAsync = nullptr;
  const HandleType m_Type;
  std::atomic<AsyncType> m_AsyncReqType = AsyncType::TYPE_UNKNOWN;
  std::atomic<HandleStatus> m_Status = HandleStatus::STATUS_UNKNOWN;
 };

 template<typename T /*uv_handle type*/>
 T* T_GET_HANDLE(UvHandle* uv_handle) {
  T* result = nullptr;
  do {
   if (!uv_handle)
    break;
   result = reinterpret_cast<T*>(uv_handle->Handle());
  } while (0);
  return result;
 }

 template<typename T = UvHandle>
 T* T_GET_HANDLE_USERDATA(void* uv_handle) {
  T* result = nullptr;
  if (uv_handle)
   result = reinterpret_cast<T*>(reinterpret_cast<uv_handle_t*>(uv_handle)->data);
  return result;
 }
 template<typename T, typename THANDLE = IHandle>
 T* T_GET_HANDLE_CALLER(void* uv_handle) {
  T* result = nullptr;
  do {
   if (!uv_handle)
    break;
   auto t_handle = reinterpret_cast<THANDLE*>(reinterpret_cast<uv_handle_t*>(uv_handle)->data);
   if (!t_handle)
    break;
   result = reinterpret_cast<T*> (t_handle->Caller());
  } while (0);
  return result;
 }
 template<typename T, typename THANDLE = UvHandle>
 T* T_GET_HANDLE_ROUTE(void* uv_handle) {
  T* result = nullptr;
  do {
   if (!uv_handle)
    break;
   auto t_handle = reinterpret_cast<THANDLE*>(reinterpret_cast<uv_handle_t*>(uv_handle)->data);
   if (!t_handle)
    break;
   result = reinterpret_cast<T*> (t_handle->Route());
  } while (0);
  return result;
 }

 template<typename T = UvHandle>
 bool T_CLOSE_HANDLE(T* handle, T* async = nullptr, const time_t& wait = 10000) {
  auto async_result = std::async(
   [&]()->bool {
    bool result = false;
    do {
     if (!handle)
      break;
     if (async) {
      async->Route(handle);
      async->AsyncReqType(AsyncType::TYPE_CLOSE_HANDLE);
      if (0 != uv_async_send(reinterpret_cast<uv_async_t*>(async->Handle()))) {
       handle->Status(HandleStatus::STATUS_ERROR);
       break;
      }
     }
     else {
      uv_close(reinterpret_cast<uv_handle_t*>(handle->Handle()),
       [](uv_handle_t* handle) {
        auto t_handle = reinterpret_cast<T*>(handle->data);
        t_handle->Status(HandleStatus::STATUS_CLOSED);
       });
     }
     std::time_t total_timeout = 0;
     do {
      if (total_timeout >= wait)
       break;
      if (handle->Status() == HandleStatus::STATUS_CLOSED) {
       result = true;
       break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      total_timeout += 100;
     } while (1);
    } while (0);
    return result;
   });
  return async_result.get();
 }





















 class Handle final {
 public:
  using tfUvWriteCb = std::function<void(uv_write_t*, int)>;
  enum class Type : unsigned long {
   UV_UNKNOWN_HANDLE = 0,
#define XX(uc, lc) UV_##uc,
   UV_HANDLE_TYPE_MAP(XX)
#undef XX
   UV_FILE,
   UV_LOOP,
   UV_CONNECT,
   UV_THREAD,
   UV_WRITE,
   UV_PIPE,
   UV_UDP_SEND,
   UV_HANDLE_TYPE_MAX
  };
 public:
  Handle(const Type&, void* caller = nullptr, Handle* async = nullptr);
  ~Handle();
 private:
  void Init(void* caller = nullptr);
  void UnInit();
 public:
  template<typename T>
  inline T* handle() const;
  const AsyncType& AsyncTypeGet() const;
  void AsyncTypeSet(const AsyncType&);
  const HandleStatus& StatusGet() const;
  void StatusSet(const HandleStatus&);
  const Type& TypeGet() const;
  bool AsyncClose(Handle*);
  bool Close();
  void Route(Handle*);
  Handle* Route() const;
  const Handle* Async() const;
  void Caller(void*);
  void* Caller() const;
  const uv_buf_t& Buf() const;
  void Buf(const char*, const size_t&);
  void WriteCb(const tfUvWriteCb&);
  void WriteCb(uv_write_t*, int) const;
  bool AsyncWriteRequest();
  void HandleUserData(void*);
  void* HandleUserData() const;
 private:
  const Type m_type;
  void* m_caller = nullptr;
  Handle* m_async = nullptr;
  Handle* m_route = nullptr;
  uv_buf_t m_uv_buf = { 0 };
  tfUvWriteCb m_UvWirteCb = nullptr;
  HandleStatus m_status = HandleStatus::STATUS_UNKNOWN;
  AsyncType m_async_req_type = AsyncType::TYPE_UNKNOWN;
  void* m_uv_handle = nullptr;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

 template<typename T>
 inline T* Handle::handle() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return reinterpret_cast<T*>(m_uv_handle);
 }

 template<typename T>
 T* T_GET_HANDLE_USER_DATA(void* uv_user_data) {
  T* result = nullptr;
  do {
   Handle* handle = reinterpret_cast<Handle*>(uv_user_data);
   if (!handle)
    break;
   auto uv_user_data = handle->HandleUserData();
   if (!uv_user_data)
    break;
   result = reinterpret_cast<T*>(uv_user_data);
  } while (0);
  return result;
 }

 //template<typename T>
 //T* T_GET_HANDLE_CALLER(void* uv_user_data) {
 // T* result = nullptr;
 // do {
 //  Handle* handle = reinterpret_cast<Handle*>(uv_user_data);
 //  if (!handle)
 //   break;
 //  auto caller = handle->Caller();
 //  if (!caller)
 //   break;
 //  result = reinterpret_cast<T*>(caller);
 // } while (0);
 // return result;
 //}


 template<typename T_HANDLE = Handle>
 bool T_SYNC_CLOSE_HANDLE(const T_HANDLE& handle, const std::time_t& timeout = 5000) {
  auto async_result = std::async(
   [&]()->bool {
    if (!handle)
     return true;
    bool result = false;
    if (handle->Close()) {
     std::time_t total_timeout = 0;
     do {
      if (total_timeout >= timeout)
       break;
      if (handle->StatusGet() == HandleStatus::STATUS_CLOSED) {
       result = true;
       break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      total_timeout += 500;
     } while (1);
    }
    return result;
   });
  return async_result.get();
 }

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 03 May 2023 02:11:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__4A69B522_AFFA_401E_AF73_A131565C1555__