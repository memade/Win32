#if !defined(__957D43A4_01DA_4EE0_8B58_F2BFD8C90734__)
#define __957D43A4_01DA_4EE0_8B58_F2BFD8C90734__

namespace local {

 class ProxyTcp final : public IProxyTcp {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  ProxyTcp();
  virtual ~ProxyTcp();
 private:
  void Init();
  void UnInit();
 protected:
  IConfig* ConfigGet() const override final;
  bool Start() override final;
  bool Ready() const override final;
  void Write(const std::string&) override final;
  void OnMessage(const tfOnServerMessage&) override final;
  void OnConnect(const tfOnConnectCb&) override final;
  void OnDisconnect(const tfOnDisconnectCb&) override final;
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
  tfOnServerMessage m_OnMessage = nullptr;
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 27 Jan 2023 00:23:32 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__957D43A4_01DA_4EE0_8B58_F2BFD8C90734__
