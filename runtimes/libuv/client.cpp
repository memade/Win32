#include "stdafx.h"

namespace local {

 Client::Client() {
  m_pLog = new shared::Log();
  m_pConfig = new Config();
 }
 Client::~Client() {
  SK_DELETE_PTR(m_pConfig);
  SK_DELETE_PTR(m_pLog);
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

 void Client::Process() {
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
     m_pSession = new Session(EnSessionType::TCP_SESSION_CLIENT);
    if (m_pSession->Status() == SessionStatus::Connecting || \
     m_pSession->Status() == SessionStatus::Closing)
     break;
    if (m_pSession->Status() == SessionStatus::Closed) {
     m_pSession->Destory();
     SK_DELETE_PTR(m_pSession);
     m_pSession = new Session(EnSessionType::TCP_SESSION_CLIENT);
    }

    m_pSession->Connect(m_pConfig->IPAddrV4(), m_pConfig->Port(),
     [&](connect_callback_route_t* route) {
      if (route->status != 0)
       m_pSession->ForceClose();

      LOG_OUTPUT(m_pLog, std::format("Connect to server {} on status({}).", route->status == 0 ? "success" : "failed", route->status));

     });
   } while (0);

   do {
    if (!m_pSession)
     break;
    if (m_pSession->Status() != ConnectionStatus::Connected)
     break;

    do {//!@ KeepAlive
     if (time_current - time_reconnection_prev < time_reconnection)
      break;
     time_reconnection_prev = time_current;

     m_pSession->Write(CommandType::KeepAlive, "Please pay attention to my existence.");
    } while (0);

    do {//read
     if (m_pSession->Status() != ConnectionStatus::Connected)
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
     LOG_OUTPUT(m_pLog,
      std::format("recv message({:X}:{})", static_cast<unsigned long>(head.Command()), message));

     switch (head.Command()) {
     case CommandType::Welcome: {
      m_pSession->Write(CommandType::Hello, std::format("Hello server ! I'm tcp client session."));
     }break;
     default:
      break;
     }
    } while (0);


    do {//write
     if (m_pSession->Status() != ConnectionStatus::Connected)
      break;
     if (!m_pSession->Write()) {
      m_pSession->ForceClose();
      break;
     }



    } while (0);



   } while (0);


   if (!m_IsOpen.load()) {
    if (m_pSession) {
     m_pSession->ForceClose();
     do {
      if (m_pSession && m_pSession->Status() == SessionStatus::Closed) {
       m_pSession->Destory();
       SK_DELETE_PTR(m_pSession);
       break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
     } while (1);
    }
    break;
   }
   time_current = ::GetTickCount64();
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (1);
 }




 Client* __gpClient = nullptr;
}///namespace local
