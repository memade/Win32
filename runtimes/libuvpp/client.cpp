#include "stdafx.h"

namespace local {

 Client::Client(const TypeIdentify& identify) : m_Identify(identify) {
  Init();
 }
 Client::~Client() {
  UnInit();
 }
 void Client::Init() {
  m_pBufferRead = new shared::container::buffer();
  m_pBufferWrite = new shared::container::buffer();
 }
 void Client::UnInit() {
  SK_DELETE_PTR(loop_);
  SK_DELETE_PTR(m_pBufferRead);
  SK_DELETE_PTR(m_pBufferWrite);
 }
 void Client::Release() const {
  delete this;
 }
 void Client::Reconnection(const time_t& interval) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_ReconnectionInterval.store(__max(0, interval));
 }
 void Client::ReconnectionCount(const std::uint16_t& count) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_ReconnectionCount.store(count);
 }
 const TypeIdentify& Client::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }
 const std::string& Client::Name() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Name;
 }
 const EnIPV& Client::Ipv() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Ipv;
 }
 const EnSocketType& Client::SocketType() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_SocketType;
 }
 const std::string& Client::ToAddr() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_ToAddr;
 }
 void Client::Write(const std::string& send_data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_pBufferWrite->push(send_data);
 }
 void Client::Write(const unsigned long long& cmd, const std::string& data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  std::string send_data;
  shared::Win::Packet::Made(cmd, data, send_data);
  m_pBufferWrite->push(send_data);
 }
 EnClientStatus Client::Status() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Status.load();
 }
 bool Client::Start(const EnSocketType& socket_type, const EnIPV& ipv, const std::string& toaddr/*127.0.0.1:13762*/) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_IsOpen.load())
    break;
   m_SocketType = socket_type;
   m_Ipv = ipv;
   m_ToAddr = toaddr;

   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Reconnection(); });
   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void Client::Stop() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_IsOpen.load())
    break;

   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }
 void Client::Process() {
  do {

   do {
    //!@ Whether the network communication is ready.
    if (EnClientStatus::ConnectSuccess != m_Status.load())
     break;
    if (m_pBufferRead->empty())
     break;
    std::string data = m_pBufferRead->front();
    if (data.empty())
     break;
    std::vector<std::string> nodes;
    size_t size_unpak = shared::Win::Packet::UnMade(data, nodes);
    if (size_unpak <= 0)
     break;
    m_pBufferRead->pop(size_unpak);
    if (m_OnClientMessage) {
     for (const auto& node : nodes) {
      m_OnClientMessage(node,
       [&](const char* send_data, const size_t& data_size) {
        if (send_data && data_size > 0)
         m_pBufferWrite->push(send_data, data_size);
       });
     }
    }

   } while (0);


   do {
    if (m_pBufferWrite->empty())
     break;
    std::string data = m_pBufferWrite->pop();
    if (data.empty())
     break;
    if (!client_)
     break;
    client_->write(data,
     [&](uv::WriteInfo write_info) {



      auto sk = 0;
     });
   } while (0);




   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   if (!m_IsOpen.load())
    break;
  } while (1);
 }

 bool Client::__uv_open() {
  do {
   if (m_Status.load() == EnClientStatus::ConnectSuccess)
    break;

   m_Status.store(EnClientStatus::Unknow);
   SK_DELETE_PTR(loop_);
   loop_ = new uv::EventLoop();
   client_ = std::make_shared<uv::TcpClient>(loop_);
   client_->setConnectStatusCallback(
    [&](uv::TcpClient::ConnectStatus status) {
     switch (status) {
     case uv::TcpClient::ConnectStatus::OnConnectSuccess: {
      m_Status.store(EnClientStatus::ConnectSuccess);
     }break;
     case uv::TcpClient::ConnectStatus::OnConnectClose: {
      m_Status.store(EnClientStatus::ConnectClose);
     }break;
     case uv::TcpClient::ConnectStatus::OnConnectFail: {
      m_Status.store(EnClientStatus::ConnectFail);
     }break;
     default: {
      m_Status.store(EnClientStatus::ConnectTimeout);
     }break;
     }
     if (m_OnClientStatus)
      m_OnClientStatus(m_Status.load());
    });
   client_->setMessageCallback(
    [&](const char* data, ssize_t size) {
     m_pBufferRead->push(data, size);
    });

   std::string ip_;
   std::uint16_t port_ = 0;
   if (!Libuv::ParseIPAddr(m_ToAddr, ip_, port_))
    break;
   uv::SocketAddr toServerAddr(ip_, port_, (uv::SocketAddr::IPV)m_Ipv);
   client_->connect(toServerAddr);

   m_pTUvloopWord = new std::thread(
    [this]() {
     loop_->run();
     loop_->runInThisLoop([&]() {});
    });

   do {
    if (m_Status.load() != EnClientStatus::Unknow)
     break;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!m_IsOpen.load())
     break;
   } while (1);

  } while (0);
  return m_Status.load() == EnClientStatus::ConnectSuccess;
 }
 void Client::__uv_close() {
  do {
   std::atomic_bool bClosed = false;
   std::thread(
    [&]() {
     loop_->runInThisLoop(
      [&]() {
       loop_->stop();
       bClosed.store(true);
      });
    }).join();
    if (m_pTUvloopWord)
     m_pTUvloopWord->join();

    do {
     if (bClosed.load())
      break;
     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } while (1);
    m_pBufferRead->clear();
    m_pBufferWrite->clear();
    SK_DELETE_PTR(m_pTUvloopWord);
  } while (0);
 }
 void Client::Reconnection() {
  std::uint16_t currcount = 0;
  do {
   do {
    if (currcount >= m_ReconnectionCount.load()) {
     //The number of reconnection attempts has reached the upper limit
     std::cout << std::format("The number of reconnection attempts has reached the upper limit.") << std::endl;
     break;
    }

    if (!__uv_open())
     __uv_close();


    ++currcount;
   } while (0);

   std::this_thread::sleep_for(\
    std::chrono::milliseconds(__min(1000, m_ReconnectionInterval.load())));
   if (!m_IsOpen.load()) {
    __uv_close();
    break;
   }
   std::cout << std::format("Preparing to reconnect to the server({}).", m_ToAddr) << std::endl;
  } while (1);
 }

 void Client::OnClientMessageGetSendData(const tfOnClientMessageGetSendData& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnClientMessageGetSendData = cb;
 }
 void Client::OnClientMessage(const tfOnClientMessage& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnClientMessage = cb;
 }
 void Client::OnClientStatus(const tfOnClientStatus& cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_OnClientStatus = cb;
 }
}///namespace local
