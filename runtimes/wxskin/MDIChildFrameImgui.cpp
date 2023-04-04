#include "stdafx.h"

namespace wx {

 IwxMDIChildFrameImgui::IwxMDIChildFrameImgui(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= L""*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/) :
  wxMDIChildFrame(parent, id, title, pos, size, style, name) {

  Bind(wxEVT_SIZE, &IwxMDIChildFrameImgui::OnSize, this);
  Bind(wxEVT_MOVE, &IwxMDIChildFrameImgui::OnMove, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameImgui::OnCloseWindow, this);
 }

 IwxMDIChildFrameImgui::~IwxMDIChildFrameImgui() {

  Unbind(wxEVT_SIZE, &IwxMDIChildFrameImgui::OnSize, this);
  Unbind(wxEVT_MOVE, &IwxMDIChildFrameImgui::OnMove, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameImgui::OnCloseWindow, this);
 }

 void IwxMDIChildFrameImgui::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameImgui::OnMove(wxMoveEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameImgui::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
}///namespace wx