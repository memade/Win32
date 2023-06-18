#include "stdafx.h"

namespace local {
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame(
  const Wxui* host,
  wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& pos,
  const wxSize& size,
  long style,
  const wxString& name)
  : wxFrame(parent, id, title, pos, size, style, name)
  , m_pWxui(host) {

 }

 Frame::~Frame() {

 }
 void Frame::Show(const bool& show) {
  wxFrame::Show(show);
 }
 HWND Frame::Hwnd() const {
  return GetHWND();
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 MDIFrame::MDIFrame(
  const Wxui* host,
  wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& pos,
  const wxSize& size,
  long style,
  const wxString& name)
  : wxMDIParentFrame(parent, id, title, pos, size, style, name)
  , m_pWxui(host) {

  Center();
  Bind(wxEVT_SIZE, &MDIFrame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &MDIFrame::OnCloseWindow, this);
 }

 MDIFrame::~MDIFrame() {
  Unbind(wxEVT_SIZE, &MDIFrame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &MDIFrame::OnCloseWindow, this);
 }
 const Wxui* MDIFrame::Host() const {
  return m_pWxui;
 }
 void MDIFrame::Show(const bool& show) {
  wxMDIParentFrame::Show(show);
 }
 HWND MDIFrame::Hwnd() const {
  return GetHWND();
 }
 IChildFrame* MDIFrame::CreateChildFrameHost(const bool& show) {
  auto pChild = new ChildFrameHost(this);
  IChildFrame* result = dynamic_cast<IChildFrame*>(pChild);
  result->Show(show);
  OnFrameCreateChildHost(result);
  pChild->SetPosition(wxPoint(10, 10)); // 设置子窗口位置
  pChild->Raise();
  ActivateNext(); // 激活子窗口
  pChild->SetSize(wxSize(200, 200)); // 设置子窗口尺寸
  return result;
 }
 IChildFrame* MDIFrame::CreateChildFrame() {
  IChildFrame* result = dynamic_cast<IChildFrame*>(new ChildFrame(this));
  OnFrameCreateChild(result);
  Refresh();
  Update();
  return result;
 }
 void MDIFrame::MDIWindowCascade() {
  Cascade();
 }
 void MDIFrame::MDIWindowTileHorz() {
  Tile(wxOrientation::wxHORIZONTAL);
 }
 void MDIFrame::MDIWindowTileVert() {
  Tile(wxOrientation::wxVERTICAL);
 }
 bool MDIFrame::MDIAppendChild(const HWND& hChild) {
  bool result = false;
  do {
   if (!hChild)
    break;
   auto parent = CreateChildFrameHost(true);
   if (!parent)
    break;
   LONG style = GetWindowLongW(hChild, GWL_STYLE);
   style &= ~(WS_CAPTION | WS_SYSMENU);
   style |= WS_POPUP;  // 设置为无边框样式
   style |= WS_CHILD;
   style |= WS_VISIBLE;
   ::SetWindowLongW(hChild, GWL_STYLE, style);

   ::SetWindowPos(hChild, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
   HWND hNewParent = ::SetParent(hChild, parent->Hwnd());
   if (!hNewParent)
    break;

   if (m_pWxui->ConfigGet()->EnableChildWindowExclusiveMode()) {
    ////!@ 适配UI位置、大小
    RECT rtParent;
    ::GetClientRect(parent->Hwnd(), &rtParent);
    ::MoveWindow(hChild, 0, 0, rtParent.right - rtParent.left, rtParent.bottom - rtParent.top, TRUE);
   }


   result = true;
  } while (0);
  return result;
 }
 void MDIFrame::RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb& cb) {
  m_OnFrameCreateChildCb = cb;
 }
 void MDIFrame::RegisterOnFrameCreateChildHostCb(const tfOnFrameCreateChildHostCb& cb) {
  m_OnFrameCreateChildHostCb = cb;
 }
 void MDIFrame::OnFrameCreateChild(IChildFrame* child) const {
  if (m_OnFrameCreateChildCb)
   m_OnFrameCreateChildCb(child);
 }
 void MDIFrame::OnFrameCreateChildHost(IChildFrame* child) const {
  if (m_OnFrameCreateChildHostCb)
   m_OnFrameCreateChildHostCb(child);
 }
 void MDIFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  auto childs = GetChildren();
  for (auto it = childs.begin(); it != childs.end(); ++it) {
   (*it)->Close();
  }

  wxQueueEvent(wxApp::GetInstance(), new wxThreadEvent(wxEVT_THREAD, WX_MSG_ON_APP_DESTROY));
  //wxEvent.Skip();
  wxEvent.Veto();
 }
 void MDIFrame::OnSize(wxSizeEvent& wxEvent) {
  if (m_pWxui->ConfigGet()->EnableChildWindowExclusiveMode()) {
   do {
    std::vector<HWND> childs;
    shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childs);
    if (childs.empty())
     break;
    for (const auto& hwnd : childs)
     ::PostMessageW(hwnd, WM_SIZE, 0, 0);
   } while (0);
  }
  wxEvent.Skip();
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////////////////////////////////////////////////////////
}///namespace local