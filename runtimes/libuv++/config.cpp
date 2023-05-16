#include "stdafx.h"

namespace local {

 Config::Config() {
  Init();
 }

 Config::~Config() {
  UnInit();
 }
 void Config::Init() {
  m_PipeName = R"(\\?\pipe\uv-test)";
  m_IPAddrV4 = "0.0.0.0";
  m_IPAddrV6 = "";
  m_Port = 8888;
 }
 void Config::UnInit() {

 }
 std::string Config::Address(const SessionType& session_type) const {
  std::string result;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  switch (session_type) {
  case SessionType::TCP_SESSION_CLIENT:
   [[fallthrough]];
  case SessionType::UDP_SESSION_CLIENT:
   [[fallthrough]];
  case SessionType::TCP_SESSION_SERVER:
   [[fallthrough]];
  case SessionType::UDP_SESSION_SERVER:
   result.append(m_IPAddrV4).append(":").append(std::to_string(m_Port));
   break;
  case SessionType::IPC_SESSION_CLIENT:
   [[fallthrough]];
  case SessionType::IPC_SESSION_SERVER:
   result = m_PipeName;
   break;
  default:
   result = "Unknown service address";
   break;
  }
  return result;
 }
 const std::string& Config::PipeName() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_PipeName;
 }
 void Config::PipeName(const std::string& pipe_name) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_PipeName = pipe_name;
 }
 const std::string& Config::IPAddrV4() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IPAddrV4;
 }
 void Config::IPAddrV4(const std::string& ipv4) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_IPAddrV4 = ipv4;
 }
 const std::string& Config::IPAddrV6() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IPAddrV6;
 }
 void Config::IPAddrV6(const std::string& ipv6) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_IPAddrV6 = ipv6;
 }
 const unsigned int& Config::Port() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Port;
 }
 void Config::Port(const unsigned int& port) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Port = port;
 }

}///namespace local
