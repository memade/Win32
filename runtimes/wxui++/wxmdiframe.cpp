#include "stdafx.h"

namespace local {

 MDIFrame::MDIFrame(
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

  Bind(wxEVT_SIZE, &MDIFrame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &MDIFrame::OnCloseWindow, this);
 }

 MDIFrame::~MDIFrame() {

  Unbind(wxEVT_SIZE, &MDIFrame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &MDIFrame::OnCloseWindow, this);
 }
 void MDIFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
 void MDIFrame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void MDIFrame::Show(const bool& show) {
  wxMDIParentFrame::Show(show);
 }
 void MDIFrame::Size(const ISize* size) {
  if (size)
   wxMDIParentFrame::SetSize(wxSize(size->width(), size->height()));
 }
 void MDIFrame::Pos(const IPos* pos) {
  if (pos)
   wxMDIParentFrame::SetPosition(wxPoint(pos->x(), pos->y()));
 }
 void MDIFrame::Align(const IAlign* align) {
  if (align)
   if (align->type() == AlignType::CENTER)
    wxMDIParentFrame::Center();
 }
 void* MDIFrame::Handle() const {
  return reinterpret_cast<void*>(const_cast<wxWindow*>(dynamic_cast<const wxWindow*>(this)));
 }
}///namespace local