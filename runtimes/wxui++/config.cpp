#include "stdafx.h"

namespace local {

 Config::Config() {
  Init();
 }

 Config::~Config() {
  UnInit();
 }

 void Config::Init() {
  m_EnableChildWindowExclusiveMode = true;
  m_bMainWindowInitialShow = true;
  m_FrameType = FrameType::MDI;
  m_SkinPath =shared::Win::PathFixedA(shared::Win::GetModulePathA() + R"(\skin\)");
 }

 void Config::UnInit() {

 }
 const char* Config::SkinPath() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_SkinPath.c_str();
 }
 void Config::SkinPath(const char* skin_path) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_SkinPath = skin_path;
 }
 void Config::RequestUrl(const char* url) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (url)
   m_RequesUrl = url;
 }
 const std::string& Config::RequestUrl() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_RequesUrl;
 }
 bool Config::MainWindowInitialShow() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_bMainWindowInitialShow;
 }
 void Config::MainWindowInitialShow(const bool& show) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_bMainWindowInitialShow = show;
 }
 void Config::EnableChildWindowExclusiveMode(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_EnableChildWindowExclusiveMode = enable;
 }
 bool Config::EnableChildWindowExclusiveMode() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_EnableChildWindowExclusiveMode;
 }
 FrameType Config::FrameTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_FrameType;
 }
 void Config::FrameTypeSet(const FrameType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_FrameType = type;
 }
}///namespace local