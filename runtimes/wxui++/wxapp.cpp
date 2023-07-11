#include "stdafx.h"

namespace local {

 IApp::IApp(Wxui* pWxui) : m_pWxui(pWxui) {
  Bind(wxEVT_THREAD, &IApp::OnAppDestory, this, WX_MSG_ON_APP_DESTROY);
 }

 IApp::~IApp() {
  Unbind(wxEVT_THREAD, &IApp::OnAppDestory, this, WX_MSG_ON_APP_DESTROY);
 }
 bool IApp::OnInit() {
  bool result = false;
  result = wxAppConsoleBase::OnInit();
  wxInitAllImageHandlers();
  return result;
 }
 Wxui* IApp::WxuiGet() const {
  return m_pWxui;
 }
 int IApp::OnExit() {
  int result = 0;
  result = wxAppConsoleBase::OnExit();
  return result;
 }
 void IApp::OnAppDestory(wxThreadEvent& event) {
  wxAppConsoleBase::ExitMainLoop();
 }


 const int WX_MSG_ON_APP_DESTROY = wxNewId();
}///namespace local