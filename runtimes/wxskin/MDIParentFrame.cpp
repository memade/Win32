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

  //m_wxAuiManager.SetManagedWindow(this);
  //m_wxAuiManager.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_DEFAULT);

#if 0
  // create a panel to hold the browser window
  wxPanel* panel = new wxPanel(this);
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(sizer);
  m_wxAuiManager.AddPane(panel, wxAuiPaneInfo().Center().Name(wxT("pane_cef")));

  // initialize CEF
  CefMainArgs args(::GetModuleHandleW(NULL));
  CefRefPtr<cef::ICefApp> app(new cef::ICefApp);
  CefSettings settings;
  CefInitialize(args, settings,app, NULL);

  // create the browser window
  CefWindowInfo window_info;
  const auto panelRect = panel->GetClientRect();
  window_info.SetAsChild(panel->GetHandle(), CefRect(panelRect.x,panelRect.y,panelRect.width,panelRect.height));
  CefBrowserSettings browser_settings;
  //CefRefPtr<cef::ICefClient> client(new cef::ICefClient::GetInstance());
  CefBrowserHost::CreateBrowser(window_info, cef::ICefClient::GetInstance(), "about:blank", browser_settings, NULL);
#endif
  //wxMenu* menu = new wxMenu;
  //SetWindowMenu(menu);

  //CreateChildNormal();

  //wxMDIChildFrame* result = new IwxMDIChildFrame(this);
  //result->Show(true);
  //m_MDIChildFrames.emplace_back(result);

  Bind(wxEVT_SIZE, &IwxMDIParentFrame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIParentFrame::OnClose, this);
 }

 IwxMDIParentFrame::~IwxMDIParentFrame() {

  Unbind(wxEVT_SIZE, &IwxMDIParentFrame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIParentFrame::OnClose, this);
  //CefShutdown();
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