#include "stdafx.h"

namespace local {

 Config::Config() {
  Init();
 }

 Config::~Config() {
  UnInit();
 }

 void Config::Init() {
  m_bMainWindowInitialShow = true;
 }

 void Config::UnInit() {

 }
 void Config::MainWindowInitialShow(const bool& show) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_bMainWindowInitialShow = show;
 }
 bool Config::MainWindowInitialShow() const {
  return m_bMainWindowInitialShow;
 }
}///namespace local