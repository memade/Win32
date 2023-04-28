#if !defined(__6EC6AC18_59BC_4031_8EAC_9EF167A3A323__)
#define __6EC6AC18_59BC_4031_8EAC_9EF167A3A323__

namespace local {

 class Session final : public ISession, public ILibuv {
  using tfConnectCb = std::function<void(connect_callback_route_t*)>;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Session(const EnSessionType&);
  virtual ~Session();
  void Init();
  void UnInit();
 public:
  void Release() const;
  bool Ready() const override final;
  SessionStatus Status() const override final;
  void Status(const SessionStatus&);
  const std::string& IP() const;
  const u_short& Port() const;
  const u_short& Key() const;
  void ForceClose();
  std::string Read();
  bool Write();
  bool Write(const CommandType& cmd, const std::string& original_data);
  bool Connect(const std::string& ip, const u_short& port, const tfConnectCb& connect_cb);
  void Destory();
  bool KeepAliveCheck(const std::uint64_t& current);
  void KeepAliveUpdate(const std::uint64_t& current);
  const EnSessionType& SessionType() const override final;
  void SetServerStream(uv_stream_t*);
 private:
  tfConnectCb m_ConnectCb = nullptr;
  Stream* m_pReadStream = nullptr;
  Stream* m_pWriteStream = nullptr;
  u_short port_;
  std::string ip_;
  const EnSessionType m_SessionType;
  std::atomic<std::uint64_t> m_TimeKeepAlive = shared::Win::Time::TimeStamp<std::chrono::milliseconds>();
  std::atomic<SessionStatus> m_Status = SessionStatus::Closed;
 private:
  static void AsyncCb(uv_async_t* async_handle);
  static void AllocCb(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);
  static void ReadCb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
  static void ConnectCb(uv_connect_t* req, int status);
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:22:54 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6EC6AC18_59BC_4031_8EAC_9EF167A3A323__

