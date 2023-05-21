#include "stdafx.h"

namespace local {

 Client::Client(const SessionType& session_type)
  : m_SessionType(session_type) {
  LOG_INIT
   m_pConfig = new Config();
 }
 Client::~Client() {
  SK_DELETE_PTR(m_pConfig);
  LOG_UNINIT;
 }
 void Client::Release() const {
  delete this;
 }
 ConnectionStatus Client::ConnStatus() const {
  if (m_pSession)
   return m_pSession->Status();
  return ConnectionStatus::Closed;
 }
 bool Client::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_Threads.emplace_back([this]() {Process(); });
   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void Client::Stop() {
  do {
   if (!m_IsOpen.load())
    break;

   m_IsOpen.store(false);
   for (auto& it : m_Threads)
    it.join();
   m_Threads.clear();
  } while (0);
 }
 IConfig* Client::ConfigGet() const {
  return dynamic_cast<IConfig*>(m_pConfig);
 }
 unsigned long Client::SessionCount() const {
  return 1;
 }
 void Client::Process() {
  LOG_OUTPUT(std::format("Start client work thread."));

  std::uint64_t time_current = ::GetTickCount64();
  std::uint64_t time_reconnection_prev = 0;
  const std::uint64_t time_reconnection = 10000;
  const std::uint64_t time_keepalive = 20000;
  do {
   do {
    if (time_current - time_reconnection_prev < time_reconnection)
     break;
    time_reconnection_prev = time_current;

    if (!m_pSession)
     m_pSession = new Session(m_SessionType);
    const auto session_status = m_pSession->Status();
    switch (session_status) {
    case SessionStatus::Closed:
     [[fallthrough]];
    case SessionStatus::Closing:
     [[fallthrough]];
    case SessionStatus::Connecting:
     break;
    case SessionStatus::ForceClose:
     LOG_OUTPUT(std::format("Disconnect from the service ({}).", m_pConfig->Address(m_SessionType)));
     m_pSession->Stop();
     SK_DELETE_PTR(m_pSession);
     m_pSession = new Session(m_SessionType);
     break;
    default:
     break;
    }

    LOG_OUTPUT(std::format("Preparing to connect to the service ({}).", m_pConfig->Address(m_SessionType)));

    m_pSession->Start(m_pConfig->Address(m_SessionType),
     [&](const bool& is_connected, bool& force_close) {
      if (!is_connected)
       force_close = true;
      LOG_OUTPUT(std::format("Connect to server {}.", is_connected ? "success" : "failed"));
     });
   } while (0);

   do {
    if (m_pSession && m_pSession->Status() != ConnectionStatus::Connected &&
     m_pSession->Status() != ConnectionStatus::Connecting)
     break;

    do {//!@ KeepAlive
     if (time_current - time_reconnection_prev < time_reconnection)
      break;
     time_reconnection_prev = time_current;

     m_pSession->Write(CommandType::KeepAlive, "Please pay attention to my existence.");
    } while (0);

    do {//read
     if (m_pSession && m_pSession->Status() != ConnectionStatus::Connected &&
      m_pSession->Status() != ConnectionStatus::Connecting)
      break;
     std::string read_data = m_pSession->Read();
     if (read_data.empty())
      break;
     HEAD head;
     std::string message;
     if (!Protocol::UnMakeStream(read_data, head, message)) {
      m_pSession->ForceClose();
      break;
     }
     LOG_OUTPUT(std::format("recv message({:X}:{})", static_cast<unsigned long>(head.Command()), message));

     switch (head.Command()) {
     case CommandType::Welcome: {
      m_pSession->Status(SessionStatus::Connected);
      m_pSession->Write(CommandType::Hello, std::format("Hello!"));
     }break;
     default:
      break;
     }
    } while (0);


    do {//write
     if (m_pSession && m_pSession->Status() != ConnectionStatus::Connected &&
      m_pSession->Status() != ConnectionStatus::Connecting)
      break;
     if (!m_pSession->Write()) {
      m_pSession->ForceClose();
      break;
     }
     time_reconnection_prev = time_current;
    } while (0);



   } while (0);


   if (!m_IsOpen.load()) {
    if (m_pSession) {
     m_pSession->Stop();
     m_pSession->Release();
    }
    break;
   }
   time_current = ::GetTickCount64();
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (1);
  LOG_OUTPUT(std::format("Exited client work thread."));
 }




 Client* __gpClient = nullptr;
}///namespace local
