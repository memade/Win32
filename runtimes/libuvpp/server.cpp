#include "stdafx.h"

namespace local {

 Server::Server() {
  Init();
 }

 Server::~Server() {
  UnInit();
 }
 void Server::Init() {
  m_loop_ = new uv::EventLoop();
  m_pUVServer = new uv::TcpServer(m_loop_);
  m_pConfig = new Config();

 }
 void Server::UnInit() {
  SK_DELETE_PTR(m_pUVServer);
  SK_DELETE_PTR(m_loop_);
  SK_DELETE_PTR(m_pConfig);
 }
 void Server::Release() const {
  delete this;
 }
 IConfig* Server::ConfigGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pConfig;
 }
 bool Server::Start() {
  do {
   if (m_IsOpen.load())
    break;

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
   if (m_OnSessionCreateAfterCb)
    m_OnSessionCreateAfterCb(dynamic_cast<ISession*>(pSession));
   else
    pSession->Write(NET_COMMAND_TCP_100201, "Welcome to server.");
    });

   m_pUVServer->setConnectCloseCallback(
    [this](std::weak_ptr<uv::TcpConnection> tcp_connection) {
     m_SessionQ.pop(tcp_connection.lock().get()->Name(),
     [&](const auto&, auto& pSession) {
       if (m_OnSessionDestoryBeforeCb)
       m_OnSessionDestoryBeforeCb(pSession);
   pSession->Release();
   if (m_OnSessionDestoryAfterCb)
    m_OnSessionDestoryAfterCb(pSession);
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
      auto sss = 0;
     } while (0);
    });

   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void Server::Stop() {
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

   if(m_ServerThread.joinable())
    m_ServerThread.join();
   m_IsOpen.store(false);
  } while (0);
 }
 bool Server::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IsOpen.load();
 }
 void Server::Write(const unsigned long long& cmd, const std::string& data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  std::string send_data;
  shared::Win::Packet::Made(cmd, data, send_data);
  m_BufferWrite.push(send_data);
 }

 void Server::Process() {
  do {
   //SendTo all users. ->m_BufferWrite
   std::string send_all_session = m_BufferWrite.pop();

   m_SessionQ.iterate(
    [&](const auto&, Session* pSession, auto&) {
     if (!send_all_session.empty())
     pSession->Write(NET_COMMAND_TCP_100200, send_all_session);
   pSession->Write();
   std::vector<std::string> session_read_s;
   pSession->Read(session_read_s);
   for (const auto& data : session_read_s)
    if (m_OnServerMessage)
     m_OnServerMessage(pSession, data);
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

 void Server::OnServerMessage(const tfOnServerMessage& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnServerMessage = cb;
 }
 void Server::OnSessionCreateAfterCb(const tfOnSessionCreateAfterCb& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnSessionCreateAfterCb = cb;
 }
 void Server::OnSessionDestoryAfterCb(const tfOnSessionDestoryAfterCb& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnSessionDestoryAfterCb = cb;
 }
 void Server::OnSessionDestoryBeforeCb(const tfOnSessionDestoryBeforeCb& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnSessionDestoryBeforeCb = cb;
 }
}///namespace local
