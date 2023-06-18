#include "stdafx.h"

namespace local {

 App::App() {

 }

 App::~App() {

 }
 bool App::OnInit() {
  bool result = false;
  result = wxAppConsoleBase::OnInit();
  wxInitAllImageHandlers();
  Bind(wxEVT_THREAD, &App::OnAppDestory, this, WX_MSG_ON_APP_DESTROY);
  OnAppInit(result);
  return result;
 }

 int App::OnExit() {
  int result = 0;
  result = wxAppConsoleBase::OnExit();
  Unbind(wxEVT_THREAD, &App::OnAppDestory, this, WX_MSG_ON_APP_DESTROY);
  OnAppUninit(result);
  return result;
 }
 void App::RegisterOnAppInitCb(const tfOnAppInitCb& cb) {
  m_OnAppInitCb = cb;
 }
 void App::RegisterOnAppUninitCb(const tfOnAppUninitCb& cb) {
  m_OnAppUninitCb = cb;
 }
 void App::RegisterOnAppCreateFrameCb(const tfOnAppCreateFrameCb& cb) {
  m_OnAppCreateFrameCb = cb;
 }
 void App::OnAppInit(const bool& result) const {
  if (m_OnAppInitCb)
   m_OnAppInitCb(result);
 }
 void App::OnAppUninit(const int& exit_code) const {
  if (m_OnAppUninitCb)
   m_OnAppUninitCb(exit_code);
 }
 void App::OnAppCreateFrame(IFrame* frame) const {
  if (m_OnAppCreateFrameCb)
   m_OnAppCreateFrameCb(frame);
 }
 void App::OnAppDestory(wxThreadEvent& event) {
  wxAppConsoleBase::ExitMainLoop();
 }


 const int WX_MSG_ON_APP_DESTROY = wxNewId();
}///namespace local