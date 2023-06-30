#include "stdafx.h"

namespace local {
#if 0
 ChildFrame::ChildFrame(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= LR"(MDIChildFrame®)"*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/)
  : wxMDIChildFrame(parent, id, title, pos, size, style, name) {

  Bind(wxEVT_SIZE, &ChildFrame::OnSize, this);
  Bind(wxEVT_SIZING, &ChildFrame::OnSizing, this);
  Bind(wxEVT_CLOSE_WINDOW, &ChildFrame::OnCloseWindow, this);
 }

 ChildFrame::~ChildFrame() {

  Unbind(wxEVT_SIZE, &ChildFrame::OnSize, this);
  Unbind(wxEVT_SIZING, &ChildFrame::OnSizing, this);
  Unbind(wxEVT_CLOSE_WINDOW, &ChildFrame::OnCloseWindow, this);
 }
 void ChildFrame::Show(const bool& show) {
  wxFrame::Show(show);
 }
 HWND ChildFrame::Hwnd() const {
  return wxFrame::GetHWND();
 }
 void ChildFrame::Maximize() {
  wxMDIChildFrame::Maximize(true);
 }
 void ChildFrame::OnSizing(wxSizeEvent& wxEvent) {

  wxEvent.Skip();
 }
 void ChildFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  std::vector<HWND> childWindows;
  shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childWindows);
  for (const auto& child : childWindows) {
   ::PostMessageW(child, WM_CLOSE, 0, 0);
  }

  wxEvent.Skip();
 }
 void ChildFrame::OnSize(wxSizeEvent& wxEvent) {
#if 0
  do {
   auto parent = wxDynamicCast(GetParent() , MDIFrame);
   if (!parent) 
    break;
   if (!parent->Host()->ConfigGet()->EnableChildWindowExclusiveMode())
    break;
   std::vector<HWND> childWindows;
   shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childWindows);
   for (const auto& child : childWindows) {
    ::PostMessageW(child, WM_SIZE, 0, 0);
   }
  } while (0);
#endif
  wxEvent.Skip();
 }








 ///////////////////////////////////////////////////////////////////////////////////////////////////////
 ChildFrameHost::ChildFrameHost(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= LR"(MDIChildFrame®)"*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/)
  : wxMDIChildFrame(parent, id, title, pos, size, style, name) {

  //SetWindowStyleFlag(wxNO_BORDER | wxFRAME_SHAPED/*wxNO_BORDER | *//*wxFRAME_SHAPED | *//*wxDEFAULT_FRAME_STYLE*/);
  //SetTransparent(128);

  Bind(wxEVT_PAINT, &ChildFrameHost::OnPaint, this);
  Bind(wxEVT_ERASE_BACKGROUND, &ChildFrameHost::OnErase, this);
  Bind(wxEVT_SIZE, &ChildFrameHost::OnSize, this);
  Bind(wxEVT_SIZING, &ChildFrameHost::OnSizing, this);
  Bind(wxEVT_CLOSE_WINDOW, &ChildFrameHost::OnCloseWindow, this);
 }

 ChildFrameHost::~ChildFrameHost() {
  Unbind(wxEVT_PAINT, &ChildFrameHost::OnPaint, this);
  Unbind(wxEVT_ERASE_BACKGROUND, &ChildFrameHost::OnErase, this);
  Unbind(wxEVT_SIZE, &ChildFrameHost::OnSize, this);
  Unbind(wxEVT_SIZING, &ChildFrameHost::OnSizing, this);
  Unbind(wxEVT_CLOSE_WINDOW, &ChildFrameHost::OnCloseWindow, this);
 }
 void ChildFrameHost::Show(const bool& show) {
  wxMDIChildFrame::Show(show);
 }
 HWND ChildFrameHost::Hwnd() const {
  return wxMDIChildFrame::GetHWND();
 }
 void ChildFrameHost::Maximize() {
  wxMDIChildFrame::Maximize(true);
 }
 void ChildFrameHost::OnSizing(wxSizeEvent& wxEvent) {

  wxEvent.Skip();
 }
 void ChildFrameHost::OnCloseWindow(wxCloseEvent& wxEvent) {
  std::vector<HWND> childWindows;
  shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childWindows);
  for (const auto& child : childWindows) {
   ::PostMessageW(child, WM_CLOSE, 0, 0);
  }

  wxEvent.Skip();
 }
 void ChildFrameHost::OnSize(wxSizeEvent& wxEvent) {

  //!@ 调整适应大小
  do {
   auto parent = GetParent();
   if (!parent)
    break;
   wxRect parentClient = parent->GetClientRect();
   SetPosition(wxPoint(parentClient.x + 10, parentClient.y + 10));
   SetSize(wxSize(parentClient.width - 22, parentClient.height - 22));
  } while (0);

  long style = GetWindowStyleFlag();
  if (style & wxFRAME_SHAPED) {
   ReSetShape();
  }

#if 0
  std::vector<HWND> childWindows;
  shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childWindows);
  auto rtParent = GetClientRect();
  for (const auto& child : childWindows) {
   ::SetWindowPos(child, HWND_TOPMOST, rtParent.x + 10, rtParent.y + 10, rtParent.width - 20, rtParent.height - 20, SWP_NOACTIVATE);
  }
#endif

#if 1
  std::vector<HWND> childWindows;
  shared::Win::Window::EnumTargetWndOfChildWnd(GetHWND(), childWindows);
  do {
   if (childWindows.empty())
    break;
   auto rtParent = GetClientRect();
   ::MoveWindow(childWindows[0], 10, 10, rtParent.width-22, rtParent.height-22, TRUE);
  } while (0);
#endif


#if 0
  for (const auto& child : childWindows) {

#if 0
   //!@ Adjust to the parent window size
   do {
    HWND hParent = ::GetParent(GetTopWindow(GetHWND()));
    if (!hParent)
     break;
    RECT rtParent;
    ::GetClientRect(hParent, &rtParent);
    SetPosition(wxPoint(rtParent.left + 10, rtParent.top + 10));
    SetSize(wxSize(rtParent.right - rtParent.left - 20, rtParent.bottom - rtParent.top - 20));
   } while (0);
#endif  
   //::PostMessageW(child, WM_SIZE, 0, 0);
  }
#endif
  wxEvent.Skip();
 }
 void ChildFrameHost::OnPaint(wxPaintEvent& wxEvent) {
  int w, h;
  GetSize(&w, &h);
  if (w == 0 || h == 0)
   return;
  //使用双缓冲绘图，防止闪烁
  wxBufferedPaintDC bufferDC(this);
  //清除背景
  bufferDC.SetBrush(*wxTRANSPARENT_BRUSH);
  bufferDC.Clear();

  wxEvent.Skip();
 }



 static wxRegion CreateRoundedRectRegion(const wxRect& rect, int radius,
  const wxColour& startColor = wxColour(255, 0, 0), const wxColour& endColor = wxColour(0, 0, 255))
 {
  wxBitmap bitmap(rect.GetWidth(), rect.GetHeight());
  wxMemoryDC memDC(bitmap);
  memDC.SetBackground(*wxWHITE);
  memDC.Clear();
  wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
  if (gc) {
   gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
   wxGraphicsPath path = gc->CreatePath();
   // 添加圆角矩形到路径中
   path.AddRoundedRectangle(rect.x, rect.y, rect.GetWidth(), rect.GetHeight(), radius);
   // 设置绘制样式
   gc->SetPen(*wxBLACK_PEN);
   gc->SetBrush(*wxBLUE_BRUSH);
   // 绘制路径的填充
   gc->FillPath(path);
  }
  SK_DELETE_PTR(gc);
  wxRegion region(bitmap, *wxWHITE);
  return region;
}

 bool ChildFrameHost::SetShape() {
  return wxNonOwnedWindow::SetShape(CreateRoundedRectRegion(GetClientRect(), 10));
 }
 void ChildFrameHost::ReSetShape() {
  int w, h;
  GetSize(&w, &h);
  if (w != 0 && h != 0)
   SetShape();
  Layout();
  Refresh();
 }
 void ChildFrameHost::OnErase(wxEraseEvent& wxEvent) {

  //wxEvent.Skip();
 }
#endif
}///namespace local