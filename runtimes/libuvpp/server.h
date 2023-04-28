#if !defined(INC_H___20F97F29_22A2_4375_9F7B_44AB07EE54EB__HEAD__)
#define INC_H___20F97F29_22A2_4375_9F7B_44AB07EE54EB__HEAD__

namespace local {

 class Server final : public IServer {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Server();
  virtual ~Server();
 private:
  void Init();
  void UnInit();
 protected:
  IConfig* ConfigGet() const override final;
  bool Start() override final;
  bool Ready() const override final;
  void Write(const unsigned long long& , const std::string&) override final;
  void OnServerMessage(const tfOnServerMessage&) override final;
  void OnSessionCreateAfterCb(const tfOnSessionCreateAfterCb&) override final;
  void OnSessionDestoryAfterCb(const tfOnSessionDestoryAfterCb&) override final;
  void OnSessionDestoryBeforeCb(const tfOnSessionDestoryBeforeCb&) override final;
 public:
  void Stop() override final;
  void Release() const override final;
 private:
  Config* m_pConfig = nullptr;
  shared::container::buffer m_BufferWrite;
  std::atomic_bool m_IsOpen = false;
  std::vector<std::thread> m_Threads;
  std::thread m_ServerThread;
  void Process();
  shared::container::map<std::string, Session*> m_SessionQ;
 private:
  uv::TcpServer* m_pUVServer = nullptr;
  uv::EventLoop* m_loop_ = nullptr;
 protected:
  tfOnConnectCb m_OnConnectCb = nullptr;
  tfOnDisconnectCb m_OnDisconnectCb = nullptr;
  tfOnServerMessage m_OnServerMessage = nullptr;
  tfOnSessionCreateAfterCb m_OnSessionCreateAfterCb = nullptr;
  tfOnSessionDestoryAfterCb m_OnSessionDestoryAfterCb = nullptr;
  tfOnSessionDestoryBeforeCb m_OnSessionDestoryBeforeCb = nullptr;
 };

}///namespace local

/// /*新生®（上海）**/
/// /*2022_10_14T22:04:54.7963906Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___20F97F29_22A2_4375_9F7B_44AB07EE54EB__HEAD__