#include "stdafx.h"

namespace local {

 Config::Config() {
  Init();
 }

 Config::~Config() {
  UnInit();
 }
 void Config::Init() {
  m_EnableSSLSupport = true;
  m_EnableDefaultPrerequestedHeader = true;
  m_ProcessExecuteInterval = 10;
  m_StatusUpdateInterval = 5000;
  m_MaximumNumberOfTasks = 5;
  m_DownCacheDirectory =\
   shared::IConv::WStringToMBytes(shared::Win::GetUserDownFolderPathW());
 }
 void Config::UnInit() {

 }
 bool Config::EnableSSLSupport() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_EnableSSLSupport;
 }
 void Config::EnableSSLSupport(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_EnableSSLSupport = enable;
 }
 bool Config::EnableDefaultPrerequestedHeader() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_EnableDefaultPrerequestedHeader;
 }
 void Config::EnableDefaultPrerequestedHeader(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_EnableDefaultPrerequestedHeader = enable;
 }
 void Config::MaximumNumberOfTasks(const std::uint8_t& num) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_MaximumNumberOfTasks = num;
 }
 std::uint8_t Config::MaximumNumberOfTasks() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_MaximumNumberOfTasks;
 }
 std::uint64_t Config::StatusUpdateInterval() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_StatusUpdateInterval;
 }
 std::uint64_t Config::ProcessExecuteInterval() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_ProcessExecuteInterval;
 }
 void Config::StatusUpdateInterval(const std::uint64_t& interval) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_StatusUpdateInterval = interval;
 }
 void Config::ProcessExecuteInterval(const std::uint64_t& interval) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_ProcessExecuteInterval = interval;
 }
 const std::string& Config::DownCacheDirectory() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_DownCacheDirectory;
 }
 void Config::DownCacheDirectory(const char* path) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_DownCacheDirectory = path ? path : "";
 }
}///namespace local
