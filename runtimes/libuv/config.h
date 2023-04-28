#if !defined(__83B7B86B_6223_4009_88E0_1C83003EF5B5__)
#define __83B7B86B_6223_4009_88E0_1C83003EF5B5__

namespace local {
 class Config : public IConfig {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Config();
  virtual ~Config();
 private:
  void Init();
  void UnInit();
 public:
  const std::string& IPAddrV4() const override final;
  void IPAddrV4(const std::string&) override final;
  const std::string& IPAddrV6() const override final;
  void IPAddrV6(const std::string&) override final;
  const unsigned int& Port() const override final;
  void Port(const unsigned int&) override final;
  const EnServerType& ServerType() const override final;
  void ServerType(const EnServerType&) override final;
  const std::string& PipeName() const override final;
  void PipeName(const std::string&) override final;
 private:
  std::string m_PipeName;
  std::string m_IPAddrV4;
  std::string m_IPAddrV6;
  unsigned int m_Port = 0;
  EnServerType m_ServerType = EnServerType::TCP;
 };
}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 15:03:30 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__83B7B86B_6223_4009_88E0_1C83003EF5B5__