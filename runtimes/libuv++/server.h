#if !defined(__16AB8F75_13DF_4230_9176_E86A1B4E8818__)
#define __16AB8F75_13DF_4230_9176_E86A1B4E8818__

namespace local {

 class Server final : public IServer {
 public:
  Server(const SessionType&);
  virtual ~Server();
 private:
  void Init();
  void UnInit();
 public:
  bool Start() override final;
  void Stop() override final;
  const SessionType& Type() const override final;
  void Release() const override final;
  IConfig* ConfigGet() const override final;
  unsigned long SessionCount() const override final;
  UvLoop* LoopHandle() const;
  UvHandle* ServerHandle() const;
 protected:
  void RegisterOnAcceptCb(const tfOnAcceptCb&) override final;
  void RegisterOnReceiveReply(const tfOnReceiveReply&) override final;
  void RegisterOnSessionAppendBeforeCb(const tfOnSessionAppendBeforeCb&) override final;
  void RegisterOnSessionRemoveBeforeCb(const tfOnSessionRemoveBeforeCb&) override final;
 private:
  void Process();
  static void OnConnect(uv_stream_t* server, int status);
  static void OnUdpAlloc(uv_handle_t* server, size_t suggested_size, uv_buf_t* buf);
  static void OnUdpRead(uv_udp_t* server, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
 private:
  const SessionType m_SessionType;
  std::atomic_bool m_IsOpen = false;
  Config* m_pConfig = nullptr;
  UvLoop* m_pLoop = nullptr;
  UvHandle* m_pServer = nullptr;
  std::vector<std::thread> m_Threads;
  shared::container::map<TypeIdentify, Session*> m_Sessions;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 protected:
  tfOnAcceptCb m_OnAcceptCb = nullptr;
  tfOnReceiveReply m_OnReceiveReply = nullptr;
  tfOnSessionAppendBeforeCb m_OnAppendBeforeCb = nullptr;
  tfOnSessionRemoveBeforeCb m_OnRemoveBeforeCb = nullptr;
 };

 extern Server* __gpServer;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:22:30 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__16AB8F75_13DF_4230_9176_E86A1B4E8818__

