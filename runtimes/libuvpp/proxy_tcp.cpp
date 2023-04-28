#include "stdafx.h"

namespace local {

 ProxyTcp::ProxyTcp() {
  Init();
 }

 ProxyTcp::~ProxyTcp() {
  UnInit();
 }
 void ProxyTcp::Init() {
  m_loop_ = new uv::EventLoop();
  m_pUVServer = new uv::TcpServer(m_loop_);
  m_pConfig = new Config();

 }
 void ProxyTcp::UnInit() {
  SK_DELETE_PTR(m_pUVServer);
  SK_DELETE_PTR(m_loop_);
  SK_DELETE_PTR(m_pConfig);
 }
 void ProxyTcp::Release() const {
  delete this;
 }
 IConfig* ProxyTcp::ConfigGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pConfig;
 }
 bool ProxyTcp::Start() {
  do {
   if (m_IsOpen.load())
    break;

#if 0//TEST HTTP CLIENT
   uv::EventLoop loop;
   auto pClientHttp = new ClientHttp(shared::Win::Time::TimeStamp<std::chrono::microseconds>(), &loop);
   pClientHttp->Start(R"(https://cn.bing.com/)", EnClientHttpConnectType::TYPE_DOMAIN);
#endif

   m_IsOpen.store(true);

   m_pUVServer->setMessageCallback([this](uv::TcpConnectionPtr tcp_connection, const char* buffer, ssize_t buffer_size) {
    m_SessionQ.search(tcp_connection->Name(),
    [&](Session* pSession) {
      pSession->Read(buffer, buffer_size);
     });
    });
   m_pUVServer->setNewConnectCallback(
    [this](std::weak_ptr<uv::TcpConnection> tcp_connection) {
     auto tcp_conn_ptr = tcp_connection.lock();

   auto pSession = new Session(
    shared::Win::Time::TimeStamp<std::chrono::microseconds>(),
    tcp_conn_ptr->Name(), tcp_conn_ptr.get());
   m_SessionQ.pop(pSession->Name(), [](const auto&, auto&) {});
   m_SessionQ.push(pSession->Name(), pSession);

   if (m_OnConnectCb) {
    m_OnConnectCb(pSession);
   }
#if 0
   if (m_OnSessionCreateAfterCb)
    m_OnSessionCreateAfterCb(dynamic_cast<ISession*>(pSession));
   else
    pSession->Write(NET_COMMAND_TCP_100201, "Welcome to server.");
#endif
    });

   m_pUVServer->setConnectCloseCallback(
    [this](std::weak_ptr<uv::TcpConnection> tcp_connection) {
     m_SessionQ.pop(tcp_connection.lock().get()->Name(),
     [&](const auto&, auto& pSession) {
       if (m_OnDisconnectCb) {
        m_OnDisconnectCb(nullptr);
       }
       pSession->Release();
      });

    });

   m_ServerThread = std::thread(
    [&]() {
     do {
      uv::SocketAddr socket_addr(
       m_pConfig->IPAddr(),
       m_pConfig->Port(),
       static_cast<uv::SocketAddr::IPV>(m_pConfig->IPV()));

      if (0 != m_pUVServer->bindAndListen(socket_addr))
       break;
      m_loop_->run();
     } while (0);
    });

   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void ProxyTcp::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   for (auto& it : m_Threads)
    it.join();
   m_Threads.clear();

   m_pUVServer->close([this]() {
    m_loop_->runInThisLoop(
     [&]() {
      m_loop_->stop();
     });
    });

   m_ServerThread.join();

  } while (0);
 }
 bool ProxyTcp::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IsOpen.load();
 }
 void ProxyTcp::Write(const std::string& data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BufferWrite.push(data);
 }

 void ProxyTcp::Process() {
  do {
   //SendTo all users. ->m_BufferWrite
   std::string send_all_session = m_BufferWrite.pop();

   m_SessionQ.iterate(
    [&](const auto&, Session* pSession, auto&) {
     std::string read_buffer;
   pSession->Read(read_buffer);
   if (m_OnMessage) {
    m_OnMessage(pSession, read_buffer);

    do {
     if (read_buffer.empty())
      break;
     size_t parsed_size = 0;
     httpparser::HttpRequest parsed_map;
     if (!httpparser::Method::parse_http_request(read_buffer, parsed_size, parsed_map))
      break;

     uv::EventLoop loop;
     auto pClientHttp = new ClientHttp(shared::Win::Time::TimeStamp<std::chrono::microseconds>(), &loop);
     pClientHttp->Start(parsed_map[std::string("host")], EnClientHttpConnectType::TYPE_DOMAIN);



     auto sk = 0;
    } while (0);



   }

     auto sk = 0;
#if 0
     if (!send_all_session.empty())
     pSession->Write(NET_COMMAND_TCP_100200, send_all_session);
   pSession->Write();
   std::vector<std::string> session_read_s;
   pSession->Read(session_read_s);
   for (const auto& data : session_read_s)
    if (m_OnMessage)
     m_OnMessage(pSession, data);
#endif
    });

   if (!m_IsOpen.load()) {
    m_SessionQ.iterate_clear(
     [&](const auto&, auto& pSession, auto&, auto& itclear) {
      pSession->Release();
    itclear = true;
     });
    break;
   }
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (1);
 }
 void ProxyTcp::OnMessage(const tfOnServerMessage& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnMessage = cb;
 }
 void ProxyTcp::OnConnect(const tfOnConnectCb& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnConnectCb = cb;
 }
 void ProxyTcp::OnDisconnect(const tfOnDisconnectCb& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnDisconnectCb = cb;
 }
}///namespace local
