#include "stdafx.h"

namespace wx {

 wxIMPLEMENT_APP_NO_MAIN(IwxApp);

 IwxApp::IwxApp() {
 }

 IwxApp::~IwxApp() {
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
   wxEvtHandler::Bind(wxEVT_THREAD, &IwxApp::OnCreateMDIChildFrame, this, WX_CMD_CreateMDIChildFrame);
   result = true;
  } while (0);
  OnAppInit(result);
  return result;
 }
 int IwxApp::OnExit() {
  int result = 0;
  do {

   wxEvtHandler::Unbind(wxEVT_THREAD, &IwxApp::OnDestory, this, WX_CMD_ONAPPDESTORY);
   wxEvtHandler::Unbind(wxEVT_THREAD, &IwxApp::OnCreateFrame, this, WX_CMD_ONAPPCREATEFRAME);
   wxEvtHandler::Unbind(wxEVT_THREAD, &IwxApp::OnShowWindow, this, WX_CMD_SHOWWINDOW);
   wxEvtHandler::Unbind(wxEVT_THREAD, &IwxApp::OnCreateMDIChildFrame, this, WX_CMD_CreateMDIChildFrame);

   result = wxApp::OnExit();
  } while (0);
  OnAppUninit(result);
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
  m_pFrame->SetTitle(event.GetString());

  OnAppCreateFrame(m_pFrame);
 }
 void IwxApp::OnDestory(wxThreadEvent& event) {
  wxAppConsoleBase::ExitMainLoop();
 }
 void IwxApp::OnShowWindow(wxThreadEvent& event) {
  if (m_pFrame)
   m_pFrame->Show(true);
 }
 void IwxApp::OnCreateMDIChildFrame(wxThreadEvent& event) {
  auto frame = wxDynamicCast(m_pFrame, IwxMDIParentFrame);
  auto ev = static_cast<IwxThreadEvent&>(event);

  if (frame)
   frame->CreateChildNormal(reinterpret_cast<HWND>(ev.RoutePtr()));
 }








}///namespace wx