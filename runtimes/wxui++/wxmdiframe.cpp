#include "stdafx.h"

namespace local {

 IMDIFrame::IMDIFrame(
  const IWxui* host,
  wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& pos,
  const wxSize& size,
  long style,
  const wxString& name)
  : wxMDIParentFrame(parent, id, title, pos, size, style, name)
  , host_(host) {

  Bind(wxEVT_SIZE, &IMDIFrame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &IMDIFrame::OnCloseWindow, this);
 }

 IMDIFrame::~IMDIFrame() {

  Unbind(wxEVT_SIZE, &IMDIFrame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IMDIFrame::OnCloseWindow, this);
 }
 void IMDIFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IMDIFrame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
}///namespace local