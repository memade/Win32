#if !defined(__0E11C1BC_0C1A_48C4_91D3_34CD094D74AA__)
#define __0E11C1BC_0C1A_48C4_91D3_34CD094D74AA__

namespace local {

 class Config final : public IConfig {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Config();
  virtual ~Config();
 private:
  void Init();
  void UnInit();
 public:
  const TypeIdentify& Identify() const override final;
  void Identify(const TypeIdentify&) override final;
  const EnIPV& IPV() const override final;
  void IPV(const EnIPV&) override final;
  const EnSocketType& SocketType() const override final;
  void SocketType(const EnSocketType&) override final;
  const std::string& IPAddr() const override final;
  void IPAddr(const std::string&) override final;
  const unsigned short& Port() const override final;
  void Port(const unsigned short&) override final;
  void Release() const override final;
 protected:
  std::string m_IPAddr;
  TypeIdentify m_Identify = 0;
  unsigned short m_Port = 0;
  EnIPV m_IPV = EnIPV::IPV4;
  EnSocketType m_SocketType = EnSocketType::TCP;
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 11 Jan 2023 13:22:07 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0E11C1BC_0C1A_48C4_91D3_34CD094D74AA__
