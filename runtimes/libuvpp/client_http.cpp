#include "stdafx.h"

namespace local {
 ClientHttp::ClientHttp(const TypeIdentify& identify, uv::EventLoop* evloop) : m_Identify(identify) {

 }
 ClientHttp::~ClientHttp() {

 }
 void ClientHttp::Release() const {
  delete this;
 }
 const TypeIdentify& ClientHttp::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }
 void ClientHttp::RoutePacket(const std::string& pak) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_RoutePacket = pak;
 }
 void ClientHttp::OnDNSGetIPAddrCb(uv::EventLoop* loop, int status, std::string& ipaddr) {
  do {
   if (status < 0)
    break;
   if (!loop->handle() || !loop->handle()->data)
    break;
   auto url_fields = reinterpret_cast<httpparser::HttpUrlFields*>(loop->handle()->data);
   uv::SocketAddr addr(ipaddr, url_fields->port());
   uv::TcpClient* client = new uv::TcpClient(loop);
   client->setMessageCallback(
    [&](const char* data, ssize_t data_size) {

     auto sk = 0;
    });
   client->setConnectStatusCallback(
    [&](uv::TcpClient::ConnectStatus status) {
     switch (status) {
     case uv::TcpClient::OnConnectSuccess: {
      do {
       if (m_RoutePacket.empty())
        break;
       client->write(m_RoutePacket);
      } while (0);
      
     }break;
     default: {

     }break;
     }
    });
   client->connect(addr);


#if 0
   if (isConnected)
   {
    client_->close([](uv::TcpClient* client)
     {
      delete client;
     });
   }
   else
   {
    delete client_;
   }
#endif
   auto sk = 0;
  } while (0);
 }
 bool ClientHttp::Start(const std::string& connect_string, const EnClientHttpConnectType& type) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  httpparser::HttpUrlFields url_fields;
  httpparser::Method::parse_http_url(connect_string, url_fields);
  uv::EventLoop* loop = new uv::EventLoop();
  loop->handle()->data = &url_fields;
  uv::DNSGet dnsGet(loop);
  dnsGet.setOnDNSCallback(
   [&](int _int, std::string& _string) {
    OnDNSGetIPAddrCb(loop, _int, _string);
   });
  int rst = dnsGet.GetIP(url_fields[httpparser::EnHttpUrlFieldType::UF_HOST], "80");

  loop->run();
  //uv::http::HttpClient* client = new uv::http::HttpClient(loop);
  //client->Req()

  do {
   if (m_IsOpen.load())
    break;
   switch (type) {
   case EnClientHttpConnectType::TYPE_IP: {

   }break;
   case EnClientHttpConnectType::TYPE_DOMAIN: {

   }break;
   default:
    break;
   }
   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void ClientHttp::Stop() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_IsOpen.load())
    break;



  } while (0);
  m_IsOpen.store(false);
 }


}///namespace local
