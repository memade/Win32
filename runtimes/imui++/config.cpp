#include "stdafx.h"

namespace local {

 Config::Config() {
  Init();
 }

 Config::~Config() {
  UnInit();
 }
 void Config::Release() const {
  delete this;
 }
 void Config::Init() {
  m_bEnableDpiAwareness = true;
  m_bMainWindowInitialShow = true;
 }

 void Config::UnInit() {

 }
 void Config::EnableDpiAwareness(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_bEnableDpiAwareness = enable;
 }
 bool Config::EnableDpiAwareness() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_bEnableDpiAwareness;
 }
 void Config::MainWindowInitialShow(const bool& show) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_bMainWindowInitialShow = show;
 }
 bool Config::MainWindowInitialShow() const {
  return m_bMainWindowInitialShow;
 }
}///namespace local