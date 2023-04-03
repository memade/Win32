#include "stdafx.h"

namespace wx {

 IwxMDIChildFrame::IwxMDIChildFrame(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= L""*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/) :
  wxMDIChildFrame(parent, id, title, pos, size, style, name) {

  Bind(wxEVT_SIZE, &IwxMDIChildFrame::OnSize, this);
  Bind(wxEVT_MOVE, &IwxMDIChildFrame::OnMove, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrame::OnCloseWindow, this);
 }

 IwxMDIChildFrame::~IwxMDIChildFrame() {

  Unbind(wxEVT_SIZE, &IwxMDIChildFrame::OnSize, this);
  Unbind(wxEVT_MOVE, &IwxMDIChildFrame::OnMove, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrame::OnCloseWindow, this);
 }

 void IwxMDIChildFrame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrame::OnMove(wxMoveEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
}///namespace wx