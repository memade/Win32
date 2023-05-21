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
  STATUS_NOREADY = STATUS_UNKNOWN,
  STATUS_ERROR = 1,
  STATUS_CLOSED = 2,
  STATUS_RUNNING = 3,
  STATUS_READY = STATUS_RUNNING,
  STATUS_CLOSING = 4,
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
  virtual uv_handle_t* Handle() const = 0;
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

 class UvHandle final : public IHandle {
 public:
  UvHandle(const HandleType&, uv_loop_t* loop = nullptr, void* caller = nullptr, void* route = nullptr);
  virtual ~UvHandle();
 protected:
  void Init() override final;
  void UnInit() override final;
 public:
  uv_handle_t* Handle() const override final;
  uv_loop_t* Loop() const;
  void Release() const override final;
  bool Activate() const override final;
  void AsyncReqType(const AsyncType&);
  AsyncType AsyncReqType() const;
  HandleType Type() const;
  void Status(const HandleStatus&);
  HandleStatus Status() const;
  void UserData(void*);
  void* UserData() const;
  void Close();
 private:
  uv_handle_t* handle_ = nullptr;
  uv_loop_t* loop_ = nullptr;
  const HandleType m_Type;
  std::atomic<AsyncType> m_AsyncReqType = AsyncType::TYPE_UNKNOWN;
  std::atomic<HandleStatus> m_Status = HandleStatus::STATUS_UNKNOWN;
 };

 class UvLoop final {
  using tfAsyncCb = std::function<void(const uv_async_t*)>;
  /*std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();*/
 public:
  UvLoop();
  virtual ~UvLoop();
 protected:
  void Init();
  void UnInit();
 public:
  uv_loop_t* Handle() const;
  int Run() const;
  bool Async(const tfAsyncCb&);
  void Close();
 private:
  uv_loop_t* loop_ = nullptr;
  uv_async_t* async_ = nullptr;
  tfAsyncCb async_cb_ = nullptr;
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
 bool T_CLOSE_HANDLE(T* handle, T* async = nullptr, const time_t& wait = 5000) {
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
      if (wait > 0) {
       if (total_timeout >= wait)
        break;
      }
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










 /////////////////////////////////////////////////////////////////////////////////////////////
 class IAbstractHandle {
 protected:
  IAbstractHandle(void* route)
   : route_(route) {}
 public:
  virtual uv_handle_t* handle() const = 0;
  virtual void uninit() = 0;
  virtual void init() = 0;
  virtual void* user_data() const = 0;
  virtual void user_data(void*) = 0;
  virtual void* route_data() const = 0;
  virtual void route_data(void*) = 0;
 protected:
  void* route_ = {};
  uv_handle_t* handle_ = {};
 };

 class TLoop {
  using tfAsyncCb = std::function<void(uv_async_t*)>;
 public:
  TLoop() { init(); }
  ~TLoop() { uninit(); }
  uv_loop_t* get() const { return loop_; }
  bool run(const tfAsyncCb& async_cb) {
   async_cb_ = async_cb;
   return 0 == uv_async_send(async_);
  }
  int run() {
   return uv_run(loop_, UV_RUN_DEFAULT);
  }
 private:
  void init() {
   uv_loop_init(loop_);
   async_->data = this;
   uv_async_init(loop_, async_,
    [](uv_async_t* async) {
     TLoop* _this = reinterpret_cast<TLoop*>(async->data);
     if (_this && _this->async_cb_)
      _this->async_cb_(async);
    });
  }
  void uninit() {
   if (loop_) {
    uv_stop(loop_);
    do {
     uv_walk(loop_, OnWalk, this);
     if (0 == run())
      break;
    } while (1);
    uv_loop_close(loop_);
    delete loop_;
    loop_ = {};
    async_ = {};
   }
  }
  static void OnClosed(uv_handle_t* handle) {
   auto i_handle = reinterpret_cast<IAbstractHandle*>(handle);
   i_handle->uninit();
   /*delete reinterpret_cast<IAbstractHandle*>(handle);*/
  }
  static void OnWalk(uv_handle_t* handle, void* arg) {
   TLoop* _this = reinterpret_cast<TLoop*>(arg);
   if (0 == uv_is_closing(handle)) {
    uv_close(handle, OnClosed);
   }
  }
  uv_loop_t* loop_ = new uv_loop_t;
  uv_async_t* async_ = new uv_async_t;
  tfAsyncCb async_cb_ = nullptr;
 };

 template<typename T,
  typename std::enable_if<
  std::is_same<T, uv_async_t>::value ||
  std::is_same<T, uv_check_t>::value ||
  std::is_same<T, uv_fs_event_t>::value ||
  std::is_same<T, uv_fs_poll_t>::value ||
  std::is_same<T, uv_idle_t>::value ||
  std::is_same<T, uv_pipe_t>::value ||
  std::is_same<T, uv_poll_t>::value ||
  std::is_same<T, uv_prepare_t>::value ||
  std::is_same<T, uv_process_t>::value ||
  std::is_same<T, uv_signal_t>::value ||
  std::is_same<T, uv_stream_t>::value ||
  std::is_same<T, uv_tcp_t>::value ||
  std::is_same<T, uv_timer_t>::value ||
  std::is_same<T, uv_tty_t>::value ||
  std::is_same<T, uv_udp_t>::value>::type* = nullptr>
 class THandle : public IAbstractHandle {
 public:
  THandle(void* udata = nullptr, void* route = nullptr)
   : IAbstractHandle(route) {
   handle_ = reinterpret_cast<decltype(handle_)>(new T{ udata });
   init();
  }
  virtual ~THandle() {
   uninit();
  }
  void* user_data() const override {
   if (!handle_)
    return nullptr;
   return handle_->data;
  }
  void user_data(void* udata) override {
   if (handle_)
    handle_->data = udata;
  }
  void* route_data() const override {
   return route_;
  }
  void route_data(void* route) override {
   route_ = route;
  }
  uv_handle_t* handle() const override { return handle_; }
  T* get() const { return reinterpret_cast<T*>(handle_); }
 protected:
  void init() override {

  }
  void uninit() override {
   if (handle_ && !uv_is_closing(handle())) {
    uv_close(handle(), OnClosed);
    handle_ = {};
   }
  }
 private:
  static void OnClosed(uv_handle_t* handle) {
   delete reinterpret_cast<T*>(handle);
  }
 };






}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 03 May 2023 02:11:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__4A69B522_AFFA_401E_AF73_A131565C1555__