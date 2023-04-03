#include "stdafx.h"

namespace wx {
 IwxMDIParentFrame::IwxMDIParentFrame(
  wxWindow* parent,
  const wxWindowID& id,
  const wxString& title,
  const wxPoint& pos,
  const wxSize& size,
  long style) :
  wxMDIParentFrame(parent, id, title, pos, size, style) {

  //wxMenu* menu = new wxMenu;
  //SetWindowMenu(menu);

  CreateChildNormal();


  Bind(wxEVT_SIZE, &IwxMDIParentFrame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIParentFrame::OnClose, this);
 }

 IwxMDIParentFrame::~IwxMDIParentFrame() {

  Unbind(wxEVT_SIZE, &IwxMDIParentFrame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIParentFrame::OnClose, this);
 }
 wxMDIChildFrame* IwxMDIParentFrame::CreateChildNormal() {
  wxMDIChildFrame* result = new IwxMDIChildFrame(this);
  result->Show(true);
  m_MDIChildFrames.emplace_back(result);
  return result;
 }
 wxMDIChildFrame* IwxMDIParentFrame::CreateChildImGui() {
  wxMDIChildFrame* result = new IwxMDIChildFrame(this);
  result->Show(true);
  m_MDIChildFrames.emplace_back(result);
  return result;
 }
 void IwxMDIParentFrame::EnableExitConfirmation(const bool& enable) {
  m_EnableExitConfirmation.store(enable);
 }
 WXHWND IwxMDIParentFrame::GetHwnd() const {
  return m_hWnd;
 }
 void IwxMDIParentFrame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIParentFrame::OnClose(wxCloseEvent& wxEvent) {
  int res = wxYES;
  if (m_EnableExitConfirmation.load()) {
   res = wxMessageBox(
    "Are you sure you want to exit system?",
    "tip",
    wxYES_NO,
    this);
  }
  if (res != wxYES) {
   wxEvent.Veto();
   return;
  }
  wxAppConsole* handle = wxApp::GetInstance();
  if (handle)
   wxQueueEvent(handle, new wxThreadEvent(wxEVT_THREAD, WX_CMD_ONAPPDESTORY));
  wxEvent.Skip();

 }


}///namespace wx