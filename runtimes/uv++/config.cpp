#include "stdafx.h"

namespace local {
 Config::Config() {
  Init();
 }
 Config::~Config() {
  UnInit();
 }
 void Config::Init() {
#if 0//_DEBUG
  m_KeepAliveMS.store(5000000);
  m_SessionTimeoutMS.store(30000000);
#else
  m_KeepAliveMS.store(20000);
  m_SessionTimeoutMS.store(30000);
#endif
  m_ClientReconnectionIntervalMS.store(10000);
  m_Address = R"(0.0.0.0:8888)";/*[0:0:0:0:0:0:0:0]:8888*/
 }
 void Config::UnInit() {

 }
 void Config::Address(const std::string& address) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Address = address;
 }
 const std::string& Config::Address() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Address;
 }
 unsigned long long Config::KeepAliveTimeMS() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_KeepAliveMS.load();
 }
 void Config::KeepAliveTimeMS(const unsigned long long& ms) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_KeepAliveMS.store(ms);
 }
 unsigned long long Config::SessionTimeoutMS() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_SessionTimeoutMS.load();
 }
 void Config::SessionTimeoutMS(const unsigned long long& ms) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_SessionTimeoutMS.store(ms);
 }
 unsigned long long Config::ClientReconnectionIntervalMS() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_ClientReconnectionIntervalMS.load();
 }
 void Config::ClientReconnectionIntervalMS(const unsigned long long& ms) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_ClientReconnectionIntervalMS.store(ms);
 }
 void Config::EnableClientWaitForTheInitialConnectionAndReceiveResult(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_EnableClientWaitForTheInitialConnectionAndReceiveResult.store(enable);
 }
 bool Config::EnableClientWaitForTheInitialConnectionAndReceiveResult() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_EnableClientWaitForTheInitialConnectionAndReceiveResult.load();
 }

}///namespace local