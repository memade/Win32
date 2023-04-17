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

  //Maximize();
  Bind(wxEVT_PAINT, &IwxMDIChildFrame::OnPaint, this);
  Bind(wxEVT_SIZE, &IwxMDIChildFrame::OnSize, this);
  Bind(wxEVT_MOVE, &IwxMDIChildFrame::OnMove, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrame::OnCloseWindow, this);
 }

 IwxMDIChildFrame::~IwxMDIChildFrame() {

  Unbind(wxEVT_PAINT, &IwxMDIChildFrame::OnPaint, this);
  Unbind(wxEVT_SIZE, &IwxMDIChildFrame::OnSize, this);
  Unbind(wxEVT_MOVE, &IwxMDIChildFrame::OnMove, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrame::OnCloseWindow, this);
 }
 void IwxMDIChildFrame::OnPaint(wxPaintEvent& wxEvent) {
#if 0
  wxPaintDC dc(this);
  wxRect rect = GetClientRect();
  wxGraphicsPath path = MakeRoundedRectangle(rect, 20);
  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
  gc->SetPen(wxPen(wxColour(0, 0, 0), 2));
  gc->StrokePath(path);
  delete gc;
#endif
  wxEvent.Skip();
 }
 void IwxMDIChildFrame::OnSize(wxSizeEvent& wxEvent) {
  //SetShape(GetRoundedRectShape());
  wxEvent.Skip();
 }
 void IwxMDIChildFrame::OnMove(wxMoveEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrame::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }

 const double wxPi = 3.1415926;
 wxGraphicsPath IwxMDIChildFrame::MakeRoundedRectangle(wxRect rect, int radius) {
  wxGraphicsPath path = wxGraphicsRenderer::GetDefaultRenderer()->CreatePath();

  int x = rect.GetX();
  int y = rect.GetY();
  int w = rect.GetWidth();
  int h = rect.GetHeight();

  // Top-left corner
  path.MoveToPoint(x + radius, y);
  path.AddArc(x + radius, y + radius, radius, wxPi, wxPi * 3 / 2, true);

  // Top-right corner
  path.AddLineToPoint(x + w - radius, y);
  path.AddArc(x + w - radius, y + radius, radius, wxPi * 3 / 2, 0, true);

  // Bottom-right corner
  path.AddLineToPoint(x + w, y + h - radius);
  path.AddArc(x + w - radius, y + h - radius, radius, 0, wxPi / 2, true);

  // Bottom-left corner
  path.AddLineToPoint(x + radius, y + h);
  path.AddArc(x + radius, y + h - radius, radius, wxPi / 2, wxPi, true);

  path.CloseSubpath();

  return path;
 }

 wxRegion IwxMDIChildFrame::GetRoundedRectShape() {
  wxRect rect = GetClientRect();
  int radius = 20;
  // Create a path for a rounded rectangle
  wxGraphicsPath path = wxGraphicsRenderer::GetDefaultRenderer()->CreatePath();
  path.AddRoundedRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), radius);
  // Create a region from the path
  wxRegion region;
  wxBitmap bitmap(rect.GetWidth(), rect.GetHeight(), 1);
  wxMemoryDC dc(bitmap);
  dc.SetBrush(*wxBLACK_BRUSH);
  dc.SetBackground(*wxBLACK_BRUSH);
  dc.Clear();
  dc.SetPen(*wxWHITE_PEN);
  dc.SetBrush(wxBrush(wxColour(0, 0, 0), wxBRUSHSTYLE_TRANSPARENT));
  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
  if (gc) {
   gc->SetAntialiasMode(wxANTIALIAS_NONE);
   gc->StrokePath(path);
   //wxPen(wxColour(0, 0, 0), 2)
   delete gc;
   //wxAlphaPixelData data(dc.GetBitmap());
   //if (data) {
   // region.Union(data, wxPoint(rect.GetLeft(), rect.GetTop()));
   //}
  }
  return region;
 }






}///namespace wx