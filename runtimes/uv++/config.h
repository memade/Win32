#if !defined(__DF04609E_A0A4_4236_8390_A763A9F4678B__)
#define __DF04609E_A0A4_4236_8390_A763A9F4678B__

namespace local {

 class Config final : public IConfig {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Config();
  ~Config();
 private:
  void Init();
  void UnInit();
 public:
  void Server(const ServerType&);
  const ServerType& Server() const override final;
  void Session(const SessionType&) override final;
  const SessionType& Session() const override final;
  void IP(const IPType&) override final;
  const IPType& IP() const override final;
  void Address(const std::string&) override final;
  const std::string& Address() const override final;
  unsigned long long SessionTimeoutMS() const override final;
  void SessionTimeoutMS(const unsigned long long&) override final;
  unsigned long long ClientReconnectionIntervalMS() const override final;
  void ClientReconnectionIntervalMS(const unsigned long long&) override final;
 private:
  IPType m_IPType;
  SessionType m_SessionType;
  ServerType m_ServerType;
  std::string m_Address;
  std::atomic_ullong m_SessionTimeoutMS;
  std::atomic_ullong m_ClientReconnectionIntervalMS;
 };
}///namespace lcoal

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 20 May 2023 01:20:31 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DF04609E_A0A4_4236_8390_A763A9F4678B__