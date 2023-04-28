#if !defined(__2834C698_237C_4998_A116_2A3B5A5236AD__)
#define __2834C698_237C_4998_A116_2A3B5A5236AD__

namespace local {

 class ClientHttp final : public IClientHttp {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  ClientHttp(const TypeIdentify&, uv::EventLoop* evloop=nullptr);
  virtual ~ClientHttp();
 public:
   bool Start(const std::string&, const EnClientHttpConnectType&) override final;
   void Stop() override final;
  const TypeIdentify& Identify() const override final;
  void Release() const override final;
  void RoutePacket(const std::string&) override final;
 private:
  void OnDNSGetIPAddrCb(uv::EventLoop*,int, std::string&);
 private:
  const TypeIdentify m_Identify;
  std::atomic_bool m_IsOpen = false;
  std::string m_RoutePacket;
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 26 Jan 2023 03:20:46 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__2834C698_237C_4998_A116_2A3B5A5236AD__