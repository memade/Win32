#include "stdafx.h"

namespace local {
 Config::Config() {
  Init();
 }
 Config::~Config() {
  UnInit();
 }
 void Config::Init() {
  m_IPAddr = R"(0.0.0.0)";
  m_Port = 8800;
  m_Identify = 0;
  m_IPV = EnIPV::IPV4;
  m_SocketType = EnSocketType::TCP;
 }
 void Config::UnInit() {

 }
 void Config::Release() const {
  delete this;
 }
 const TypeIdentify& Config::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }
 void Config::Identify(const TypeIdentify& identify) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Identify = identify;
 }
 const EnIPV& Config::IPV() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IPV;
 }
 void Config::IPV(const EnIPV& ipv) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_IPV = ipv;
 }
 const EnSocketType& Config::SocketType() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_SocketType;
 }
 void Config::SocketType(const EnSocketType& type) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_SocketType = type;
 }
 const std::string& Config::IPAddr() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IPAddr;
 }
 void Config::IPAddr(const std::string& addr) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_IPAddr = addr;
 }
 const unsigned short& Config::Port() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Port;
 }
 void Config::Port(const unsigned short& port) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_Port = port;
 }

}///namespace local