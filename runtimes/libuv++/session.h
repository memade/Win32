#if !defined(__6EC6AC18_59BC_4031_8EAC_9EF167A3A323__)
#define __6EC6AC18_59BC_4031_8EAC_9EF167A3A323__

namespace local {
 class Server;
 class Session final : public ISession {
  using tfConnectCb = std::function<void(const bool&, bool&)>;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Session(const SessionType&, Server* server = nullptr, const struct sockaddr* psockaddr = nullptr);
  virtual ~Session();
 private:
  void Init();
  void UnInit();
 public:
  void Release() const;
  bool Ready() const override final;
  SessionStatus Status() const override final;
  void Status(const SessionStatus&);
  const std::string& IP() const;
  const u_short& Port() const;
  void ForceClose();
  bool Start(const std::string& address = "", const tfConnectCb& connect_cb = nullptr);
  void Stop();
  std::string Read();
  bool Read(const char*, const size_t&);
  bool Write();
  bool Write(const CommandType& cmd, const std::string& original_data);
  bool Connect(const std::string& address, const tfConnectCb& connect_cb);
  bool KeepAliveCheck(const std::uint64_t& current);
  void KeepAliveUpdate(const std::uint64_t& current);
  const SessionType& Type() const override final;
  const std::string& Address() const override final;
  void Address(const std::string&);
  const TypeIdentify& Identify() const override final;
  const struct sockaddr& SockAddr() const;
  UvHandle* ClientHandle() const;
 private:
  Server* m_pServer = nullptr;
  UvLoop* m_pUvLoop = nullptr;
  UvHandle* m_pUvAsync = nullptr;
  UvHandle* m_pUvClient = nullptr;
  uv_connect_t* m_pUvConnect = nullptr;
  std::atomic<SessionStatus> m_Status = SessionStatus::Unready;
  const SessionType m_SessionType;
  u_short m_Port = 0;
  std::string m_IPV4;
  std::string m_IPV6;
  std::string m_Address;
  tfConnectCb m_ConnectCb = nullptr;
  Stream* m_pReadStream = nullptr;
  Stream* m_pWriteStream = nullptr;
  struct sockaddr m_SockAddr = { 0 };
  TypeIdentify m_Identify = 0;
  std::vector<std::thread> m_Threads;
  std::atomic<std::uint64_t> m_TimeKeepAlive = shared::Win::Time::TimeStamp<std::chrono::milliseconds>();
 private:
  bool Hello();
  bool Welcome();
  void ConnectCb(const bool&, bool&) const;
 private:
  static void AllocCb(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);
  static void ReadCb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
  static void ConnectCb(uv_connect_t* req, int status);
  static void OnUdpReadCb(uv_udp_t* server, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:22:54 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6EC6AC18_59BC_4031_8EAC_9EF167A3A323__

