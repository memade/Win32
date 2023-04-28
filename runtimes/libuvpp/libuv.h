#include "stdafx.h"

namespace local {

 class Libuv final : public ILibuv {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Libuv();
  virtual ~Libuv();
 protected:
  IServer* CreateServer() override final;
  IClient* CreateClient() override final;
  IServerHttp* CreateServerHttp() override final;
  IClientHttp* CreateClientHttp() override final;
  IProxyTcp* CreateProxyTcp() override final;
  void Release() const override final;
 private:
  ServerHttp* m_pServerHttp = nullptr;
  ProxyTcp* m_pProxyTcp = nullptr;
  Server* m_pServer = nullptr;
  shared::container::map<TypeIdentify, Client*> m_ClientQ;
 public:
  static bool ParseIPAddr(const std::string&, std::string&, std::uint16_t&);
 };


 extern Libuv* __gpLibuv;
}///namespace local