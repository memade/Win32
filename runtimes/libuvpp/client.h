#if !defined(INC_H___C69A9913_8831_4CAE_9159_CE7FE73ABE11__HEAD__)
#define INC_H___C69A9913_8831_4CAE_9159_CE7FE73ABE11__HEAD__

namespace local {
 class Client final : public IClient {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Client(const TypeIdentify&);
  virtual ~Client();
 private:
  void Init();
  void UnInit();
 private:
  bool __uv_open();
  void __uv_close();
 protected:
  const TypeIdentify& Identify() const override final;
  const std::string& Name() const override final;
  const EnIPV& Ipv() const override final;
  const EnSocketType& SocketType() const override final;
  const std::string& ToAddr() const override final;
  bool Start(const EnSocketType&, const EnIPV&, const std::string&) override final;
  EnClientStatus Status() const override final;
  void Reconnection(const time_t&) override final;
  void ReconnectionCount(const std::uint16_t&) override final;
  void Write(const unsigned long long& , const std::string&) override final;
  void Write(const std::string&) override final;
  void OnClientMessageGetSendData(const tfOnClientMessageGetSendData&) override final;
  void OnClientMessage(const tfOnClientMessage&) override final;
  void OnClientStatus(const tfOnClientStatus&) override final;
 public:
  void Stop() override final;
  void Release() const override final;
 private:
  std::thread* m_pTUvloopWord = nullptr;
  std::atomic<EnClientStatus> m_Status = EnClientStatus::Unknow;
  std::atomic_bool m_IsOpen = false;
  //!@ m_ReconnectionCount == 0 Infinite reconnection
  std::atomic_uint16_t m_ReconnectionCount = 1; 
  std::atomic_llong m_ReconnectionInterval = 5000;//ms
  const TypeIdentify m_Identify;
  std::string m_Name;
  EnIPV m_Ipv = EnIPV::IPV4;
  EnSocketType m_SocketType = EnSocketType::TCP;
  std::string m_ToAddr;
  shared::container::buffer* m_pBufferRead = nullptr;
  shared::container::buffer* m_pBufferWrite = nullptr;
  std::vector<std::thread> m_Threads;
 private:
  uv::EventLoop* loop_ = nullptr;
  uv::TcpClientPtr client_ = nullptr;
  void Process();
  void Reconnection();
 protected:
  tfOnClientMessageGetSendData m_OnClientMessageGetSendData = nullptr;
  tfOnClientMessage m_OnClientMessage = nullptr;
  tfOnClientStatus m_OnClientStatus = nullptr;
 };

}///namespace local

/// /*新生®（上海）**/
/// /*2022_10_14T22:04:43.3935835Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___C69A9913_8831_4CAE_9159_CE7FE73ABE11__HEAD__