#include "stdafx.h"

namespace wx {

 IwxMDIChildFrameNormal::IwxMDIChildFrameNormal(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= L""*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/) :
  wxMDIChildFrame(parent, id, title, pos, size, style, name) {
  auto hwnd = GetHWND();
  static_cast<IwxApp*>(wxApp::GetInstance())->OnAppCreateFrameChild(hwnd);
  Bind(wxEVT_SIZE, &IwxMDIChildFrameNormal::OnSize, this);
  Bind(wxEVT_MOVE, &IwxMDIChildFrameNormal::OnMove, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameNormal::OnCloseWindow, this);
 }

 IwxMDIChildFrameNormal::~IwxMDIChildFrameNormal() {

  Unbind(wxEVT_SIZE, &IwxMDIChildFrameNormal::OnSize, this);
  Unbind(wxEVT_MOVE, &IwxMDIChildFrameNormal::OnMove, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameNormal::OnCloseWindow, this);
 }

 void IwxMDIChildFrameNormal::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameNormal::OnMove(wxMoveEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameNormal::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
}///namespace wx