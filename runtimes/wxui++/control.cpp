#include "stdafx.h"

namespace local {
 Control::Control() {

 }
 Control::~Control() {

 }

 ////////////////////////////////////////////////////////////////////////////////////////////////
 Window::Window(const Wxui* host) : host_(host) {
  //Handle(dynamic_cast<wxMDIParentFrame*>(this));
  initial_pointer_ = reinterpret_cast<decltype(initial_pointer_)>(this);

  wxWindow::Bind(wxEVT_SIZE, &Window::OnSize, this);
  wxWindow::Bind(wxEVT_CLOSE_WINDOW, &Window::OnCloseWindow, this);
 }
 Window::~Window() {

  wxWindow::Unbind(wxEVT_SIZE, &Window::OnSize, this);
  wxWindow::Unbind(wxEVT_CLOSE_WINDOW, &Window::OnCloseWindow, this);
  SK_DESTROY_ICON(logo_icon_);
 }
 void Window::Release() const {
  delete this;
 }
 Vec2 Window::GetSize() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  const wxSize size = wxMDIParentFrame::GetClientSize();
  return Vec2(size.x, size.y);
 }
#if 0
 void Window::SetLogo(const ICache* icon_cache) {
  std::lock_guard<std::mutex>(*m_Mutex);
  if (icon_cache) {
   logo_icon_ = shared::Win::Window::LoadMemIcon(std::string(icon_cache->Buffer(), icon_cache->Size()));
   if (logo_icon_)
    shared::Win::Window::SetLogo2(GetHWND(), logo_icon_);
  }
 }
 void Window::Center() {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::Center();
 }
 void Window::SetSize(const Vec2& size) {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::SetSize(wxSize(size.x, size.y));
 }
 void Window::Refresh() {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::Refresh();
 }
 void Window::Visible(const bool& show) {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::Show(show);
 }
 bool Window::Visible() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return wxMDIParentFrame::IsVisible();
 }
 void Window::Maximize() {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::Maximize();
 }
 void Window::BkColor(const Vec4& argb) {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::SetBackgroundColour(wxColour(argb.y * 255, argb.z * 255, argb.w * 255));
  wxMDIParentFrame::SetTransparent(argb.x * 255);
 }
 void Window::Restore() {
  std::lock_guard<std::mutex>(*m_Mutex);
  wxMDIParentFrame::Restore();
 }
 void Window::SetTitle(const IString* text) {
  std::lock_guard<std::mutex>(*m_Mutex);
  if (text)
   wxMDIParentFrame::SetTitle(wxString(text->wstr()));
 }
#endif
 void Window::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   if (1 != wxMDIParentFrame::Create(
    nullptr,
    wxID_ANY,
    LR"(MDIParentFrame®)",
    wxDefaultPosition,
    wxDefaultSize,
    wxDEFAULT_FRAME_STYLE | wxFRAME_NO_WINDOW_MENU,
    wxASCII_STR(wxFrameNameStr)))
    break;

   if (!attribute_s_)
    break;

   attribute_s_->Iterator(
    [&](const AttributeType& type, IAttribute* style, bool& itbreak) {
     switch (type) {
     case AttributeType::alpha: {

     }break;
     case AttributeType::logo: {
      auto res = dynamic_cast<const IResource*>(style->ResourceRef());
      if (!res)
       break;
      auto atts = res->GetAttributes();
      if (!atts)
       break;
      auto icon_cache = atts->GetCache();
      if (!icon_cache)
       break;
      logo_icon_ = shared::Win::Window::LoadMemIcon(std::string(icon_cache->Buffer(), icon_cache->Size()));
      if (!logo_icon_)
       break;
      shared::Win::Window::SetLogo2(m_hWnd, logo_icon_);
     }break;
     case AttributeType::size: {
      const auto size = style->GetVec2();
      wxMDIParentFrame::SetSize(wxSize(size.x, size.y));
     }break;
     case AttributeType::bkcolor: {
      const auto argb = style->GetVec4();
      wxMDIParentFrame::SetBackgroundColour(wxColour(argb.y * 255, argb.z * 255, argb.w * 255));
      wxMDIParentFrame::SetTransparent(argb.x * 255);
     }break;
     case AttributeType::visible: {
      wxMDIParentFrame::Show(style->GetBool());
     }break;
     case AttributeType::center: {
      if (!style->GetBool())
       break;
      Center();
     }break;
     case AttributeType::title_name: {
      const auto title_text = style->GetString();
      if (!title_text)
       break;
      wxMDIParentFrame::SetTitle(wxString(title_text->wstr()));
     }break;
     default:
      break;
     }
    });

  } while (0);
 }
 void Window::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
 }
 void Window::OnCloseWindow(wxCloseEvent& wxEvent) {
  const_cast<Wxui*>(host_)->SkinDestroy();
  wxEvent.Veto();
 }
 void Window::OnSize(wxSizeEvent& wxEvent) {
  do {
   if (!host_ || !host_->SkinGet())
    break;
   host_->SkinGet()->Layout();
  } while (0);
  wxEvent.Skip();
 }
 void Window::OnLayout() {

 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame() {
  initial_pointer_ = reinterpret_cast<decltype(initial_pointer_)>(this);

  wxWindow::Bind(wxEVT_SIZE, &Frame::OnSize, this);
  wxWindow::Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);
 }
 Frame::~Frame() {

  wxWindow::Unbind(wxEVT_SIZE, &Frame::OnSize, this);
  wxWindow::Unbind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);
 }
 void Frame::Release() const {
  delete this;
 }
 void Frame::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = dynamic_cast<wxMDIParentFrame*>(parent);
   if (!parent_handle)
    break;

   if (!wxMDIChildFrame::Create(
    parent_handle,
    wxID_ANY,
    LR"(MDIChildFrame®))",
    wxDefaultPosition,
    wxDefaultSize,
    /*wxDEFAULT_FRAME_STYLE | wxNO_BORDER*/wxPOPUP_WINDOW,
    wxASCII_STR(wxFrameNameStr)))
    break;

   wxMDIChildFrame::Show();
  } while (0);
 }
 void Frame::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
 }
 void Frame::OnCloseWindow(wxCloseEvent& wxEvent) {
  wxEvent.Skip();
 }
 void Frame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void Frame::OnLayout() {
  wxMDIChildFrame::SetSize(wxSize(layout_data_.z, layout_data_.w));
  wxMDIChildFrame::SetPosition(wxPoint(layout_data_.x, layout_data_.y));
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 VerticalLayout::VerticalLayout() {
  initial_pointer_ = reinterpret_cast<decltype(initial_pointer_)>(this);
  wxWindow::Bind(wxEVT_SIZE, &VerticalLayout::OnSize, this);
 }
 VerticalLayout::~VerticalLayout() {
  wxWindow::Unbind(wxEVT_SIZE, &VerticalLayout::OnSize, this);
 }

 void VerticalLayout::Release() const {
  delete this;
 }
 void VerticalLayout::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = dynamic_cast<wxWindow*>(parent);
   if (!parent_handle)
    break;
   if (!wxPanel::Create(
    parent_handle,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    wxTAB_TRAVERSAL | wxNO_BORDER,
    wxASCII_STR(wxFrameNameStr)))
    break;
   const auto argb = attribute_s_->GetBkColor();
   wxPanel::SetBackgroundColour(wxColour(argb.y * 255, argb.z * 255, argb.w * 255));
   wxPanel::SetTransparent(argb.x * 255);
   wxPanel::Show();
  } while (0);
 }
 void VerticalLayout::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
 }
 void VerticalLayout::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void VerticalLayout::OnLayout() {
  wxPanel::SetSize(wxSize(layout_data_.z, layout_data_.w));
  wxPanel::SetPosition(wxPoint(layout_data_.x, layout_data_.y));
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 HorizontalLayout::HorizontalLayout() {
  //Handle(dynamic_cast<wxPanel*>(this));
  initial_pointer_ = reinterpret_cast<decltype(initial_pointer_)>(this);

 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }
 void HorizontalLayout::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = dynamic_cast<wxWindow*>(parent);
   if (!parent_handle)
    break;
   if (!wxPanel::Create(
    parent_handle,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    wxTAB_TRAVERSAL | wxNO_BORDER,
    wxASCII_STR(wxFrameNameStr)))
    break;
   const auto argb = attribute_s_->GetBkColor();
   wxPanel::SetBackgroundColour(wxColour(argb.y * 255, argb.z * 255, argb.w * 255));
   wxPanel::SetTransparent(argb.x * 255);
   wxPanel::Show();
  } while (0);
 }
 void HorizontalLayout::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
 }
 void HorizontalLayout::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void HorizontalLayout::OnLayout() {
  wxPanel::SetSize(wxSize(layout_data_.z, layout_data_.w));
  wxPanel::SetPosition(wxPoint(layout_data_.x, layout_data_.y));
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 Button::Button() {
  initial_pointer_ = reinterpret_cast<decltype(initial_pointer_)>(this);

 }
 Button::~Button() {

 }
 void Button::Release() const {
  delete this;
 }
 void Button::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = dynamic_cast<wxWindow*>(parent);
   if (!parent_handle)
    break;
   if (!wxButton::Create(
    parent_handle,
    wxID_ANY))
    break;
   const auto argb = attribute_s_->GetBkColor();
   wxButton::SetBackgroundColour(wxColour(argb.y * 255, argb.z * 255, argb.w * 255));
   wxButton::SetTransparent(argb.x * 255);
   wxButton::Show();
  } while (0);
 }
 void Button::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
 }
 void Button::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void Button::OnLayout() {
  wxButton::SetSize(wxSize(layout_data_.z, layout_data_.w));
  wxButton::SetPosition(wxPoint(layout_data_.x, layout_data_.y));
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
 ////////////////////////////////////////////////////////////////////////////////////////////////
 Node::Node() {

 }
 Node::~Node() {

 }
 void Node::OnLayout() const {

 }
 void Node::NodeSK(INode* node) {
  node_quantum_ = node;
 }
 void Node::NodeSK(INodeSK* node) {
  node_quantum_ = dynamic_cast<INode*>(node);
 }
 INodeSK* Node::NodeSK() const {
  return dynamic_cast<INodeSK*>(node_quantum_);
 }
 void Node::Parent(INodeUI* node) {
  parent_ = dynamic_cast<INode*>(node);
 }
 INodeUI* Node::Parent() const {
  return dynamic_cast<INodeUI*>(parent_);
 }
 void Node::Refresh() {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->Refresh();
  } while (0);
 }
 void Node::Center() {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->Center();
  } while (0);
 }
 void Node::SetMaxSize(const Vec2& size) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->SetMaxSize(wxSize(size.x, size.y));
  } while (0);
 }
 void Node::SetMinSize(const Vec2& size) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->SetMinSize(wxSize(size.x, size.y));
  } while (0);
 }
 void Node::Maximize() {

 }
 void Node::Restore() {

 }
 void Node::SetLogo(const HICON& icon) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   HWND hwnd = handle->GetHWND();
   if (!hwnd)
    break;
   shared::Win::Window::SetLogo2(hwnd, icon);
  } while (0);
 }
 void Node::SetSize(const Vec2& size) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->SetSize(wxSize(size.x, size.y));
  } while (0);
 }
 void Node::Show(const bool& show_flag) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->Show(show_flag);
  } while (0);
 }
 void Node::SetBkColor(const Vec4& vec) {
  do {
   auto handle = reinterpret_cast<wxWindow*>(Handle());
   if (!handle)
    break;
   handle->SetBackgroundColour(wxColour(vec.y * 255, vec.z * 255, vec.w * 255));
   handle->SetTransparent(vec.x * 255);
  } while (0);
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 Window::Window() {
  Handle(dynamic_cast<wxMDIParentFrame*>(this));

  Bind(wxEVT_SIZE, &Window::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &Window::OnCloseWindow, this);
 }
 Window::~Window() {

  Unbind(wxEVT_SIZE, &Window::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &Window::OnCloseWindow, this);
 }
 void Window::Release() const {
  delete this;
 }
 void Window::Maximize() {
  wxMDIParentFrame::Maximize();
 }
 void Window::Restore() {
  wxMDIParentFrame::Restore();
 }
 void Window::SetTitle(const IString* text) {
  if (text)
   wxMDIParentFrame::SetTitle(wxString(text->wstr()));
 }
 void Window::OnRenderBegin(const tfOnRenderStyleCb& onRenderCb) {
  do {
   if (1 != wxMDIParentFrame::Create(
    nullptr,
    wxID_ANY,
    LR"(MDIParentFrame®)",
    wxDefaultPosition,
    wxDefaultSize,
    wxDEFAULT_FRAME_STYLE | wxFRAME_NO_WINDOW_MENU,
    wxASCII_STR(wxFrameNameStr)))
    break;
   if (onRenderCb)
    onRenderCb();
  } while (0);
 }
 void Window::OnRenderEnd() {
  auto sk = 0;
 }
 void Window::OnCloseWindow(wxCloseEvent& wxEvent) {
  do {
   auto host = reinterpret_cast<Wxui*>(Host());
   if (!host)
    break;
   host->SkinDestroy();
  } while (0);
  wxEvent.Veto();
 }
 void Window::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame() {
  Handle(dynamic_cast<wxMDIChildFrame*>(this));

  Bind(wxEVT_SIZE, &Frame::OnSize, this);
  Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);
 }
 Frame::~Frame() {

  Unbind(wxEVT_SIZE, &Frame::OnSize, this);
  Unbind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);
 }
 void Frame::Release() const {
  delete this;
 }
 void Frame::SetTitle(const IString* text) {
  if (text)
   wxMDIChildFrame::SetTitle(wxString(text->wstr()));
 }
 void Frame::Maximize() {
  wxMDIChildFrame::Maximize();
 }
 void Frame::Restore() {
  wxMDIChildFrame::Restore();
 }
 void Frame::OnRenderBegin(const tfOnRenderStyleCb& onRenderCb) {
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = reinterpret_cast<wxMDIParentFrame*>(parent->Handle());
   if (!parent_handle)
    break;
   if (!wxMDIChildFrame::Create(
    parent_handle,
    wxID_ANY,
    LR"(MDIChildFrame®))",
    wxDefaultPosition,
    wxDefaultSize,
    wxDEFAULT_FRAME_STYLE,
    wxASCII_STR(wxFrameNameStr)))
    break;

   if (onRenderCb)
    onRenderCb();
  } while (0);
 }
 void Frame::OnRenderEnd() {
  auto sk = 0;
 }
 void Frame::OnCloseWindow(wxCloseEvent& wxEvent) {
  auto sk = 0;
 }
 void Frame::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 VerticalLayout::VerticalLayout() {
  Handle(dynamic_cast<wxPanel*>(this));

 }
 VerticalLayout::~VerticalLayout() {

 }

 void VerticalLayout::Release() const {
  delete this;
 }
 void VerticalLayout::OnRenderBegin(const tfOnRenderStyleCb& onRenderCb) {
  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = reinterpret_cast<wxWindow*>(parent->Handle());
   if (!parent_handle)
    break;
   if (!wxPanel::Create(
    parent_handle,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    wxTAB_TRAVERSAL | wxNO_BORDER,
    wxASCII_STR(wxFrameNameStr)))
    break;
   if (onRenderCb)
    onRenderCb();
  } while (0);
 }
 void VerticalLayout::OnRenderEnd() {

 }
 void VerticalLayout::OnCloseWindow(wxCloseEvent& wxEvent) {

 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 HorizontalLayout::HorizontalLayout() {
  Handle(dynamic_cast<wxPanel*>(this));

 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }
 void HorizontalLayout::OnRenderBegin(const tfOnRenderStyleCb& onRenderCb) {

  do {
   auto parent = Parent();
   if (!parent)
    break;
   auto parent_handle = reinterpret_cast<wxWindow*>(parent->Handle());
   if (!parent_handle)
    break;
   if (!wxPanel::Create(
    parent_handle,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    wxTAB_TRAVERSAL | wxNO_BORDER,
    wxASCII_STR(wxFrameNameStr)))
    break;
   if (onRenderCb)
    onRenderCb();
  } while (0);
 }
 void HorizontalLayout::OnRenderEnd() {

 }
 void HorizontalLayout::OnCloseWindow(wxCloseEvent& wxEvent) {

 }
 ////////////////////////////////////////////////////////////////////////////////////////////////
 TreeList::TreeList(wxWindow* parent,
  wxWindowID id,
  const wxPoint& pos,
  const wxSize& size,
  long style,
  const wxString& name) :
  wxTreeListCtrl(parent, id, pos, size, style, name) {
  Init();
 }

 TreeList::~TreeList() {
  UnInit();
 }

 void TreeList::Init() {

 }

 void TreeList::UnInit() {

 }
 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////
#endif
 }///namespace local