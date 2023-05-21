#include "stdafx.h"

namespace wx {

 wxIMPLEMENT_APP_NO_MAIN(IwxApp);

 IwxApp::IwxApp() {

 }
 IwxApp::~IwxApp() {
 }
 void IwxApp::RegisterAppInitEventCb(const tfAppInitEventCallback& cb) {
  m_AppInitEventCb = cb;
 }
 void IwxApp::RegisterAppCloseEventNotifyCb(const tfAppCloseEventNotifyCallback& cb) {
  m_AppCloseEventNotifyCb = cb;
 }
 void IwxApp::RegisterAppCreateFrameEventCb(const tfAppCreateFrameEventCallback& cb) {
  m_AppCreateFrameEventCb = cb;
 }
 bool IwxApp::OnInit() {
  bool result = false;
  do {
   if (!wxApp::OnInit())
    break;
   wxInitAllImageHandlers();
   wxAppBase::SetExitOnFrameDelete(false);
   wxEvtHandler::Bind(wxEVT_THREAD, &IwxApp::OnDestory, this, WX_CMD_ONAPPDESTORY);
   wxEvtHandler::Bind(wxEVT_THREAD, &IwxApp::OnCreateFrame, this, WX_CMD_ONAPPCREATEFRAME);
   wxEvtHandler::Bind(wxEVT_THREAD, &IwxApp::OnShowWindow, this, WX_CMD_SHOWWINDOW);

   result = true;
  } while (0);
  if (m_AppInitEventCb)
   m_AppInitEventCb(result);
  return result;
 }
 int IwxApp::OnExit() {
  int result = 0;
  do {

   result = wxApp::OnExit();

   if (m_AppCloseEventNotifyCb)
    m_AppCloseEventNotifyCb();
  } while (0);
  return result;
 }
 wxFrame* IwxApp::FrameGet() const {
  return m_pFrame;
 }
 void IwxApp::OnCreateFrame(wxThreadEvent& event) {
  auto obj = event.GetEventObject();
  if (obj) {
   m_pFrame = wxDynamicCast(obj, wxFrame);
  }
  else {
   m_pFrame = new IwxMDIParentFrame();
   m_pFrame->Show(true);
  }

  if (m_AppCreateFrameEventCb)
   m_AppCreateFrameEventCb(m_pFrame);
 }
 void IwxApp::OnDestory(wxThreadEvent& event) {
  wxAppConsoleBase::ExitMainLoop();
 }
 void IwxApp::OnShowWindow(wxThreadEvent& event) {
  if (m_pFrame)
   m_pFrame->Show(true);
 }

}///namespace wx