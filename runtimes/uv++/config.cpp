#include "stdafx.h"

namespace local {
 Config::Config() {
  Init();
 }
 Config::~Config() {
  UnInit();
 }
 void Config::Init() {
  m_IPType = IPType::IPPROTO_IPV4;
  m_SessionType = SessionType::TCP;
  m_ServerType = ServerType::ACCEPTOR;
  m_KeepAliveMS.store(5000);
  m_SessionTimeoutMS.store(30000);
  m_ClientReconnectionIntervalMS.store(10000);
  m_Address = R"(0.0.0.0:8888)";
 }
 void Config::UnInit() {

 }
 void Config::Server(const ServerType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_ServerType = type;
 }
 const ServerType& Config::Server() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_ServerType;
 }
 void Config::Session(const SessionType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_SessionType = type;
 }
 const SessionType& Config::Session() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_SessionType;
 }
 void Config::IP(const IPType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_IPType = type;
 }
 const IPType& Config::IP() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_IPType;
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


}///namespace local