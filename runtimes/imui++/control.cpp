#include "stdafx.h"

namespace local {

 Control::Control() {

 }
 Control::~Control() {
  SK_DESTROY_ICON(logo_icon_);
 }
 void Control::PushStyleVar(const ImGuiStyleVar& style, const Vec2& var) {
  ImGui::PushStyleVar(style, ImVec2(var.x, var.y));
  pop_style_var_++;
 }
 void Control::PushStyleVar(const ImGuiStyleVar& style, const TypePixels& var) {
  ImGui::PushStyleVar(style, var);
  pop_style_var_++;
 }
 void Control::PushStyleCol(const ImGuiCol_& style, const Vec4& argb) {
  ImGui::PushStyleColor(style, ImVec4{ argb.y, argb.z, argb.w, argb.x });
  pop_style_color_++;
 }
 void Control::PopStyle() {
  ImGui::PopStyleColor(pop_style_color_.load());
  ImGui::PopStyleVar(pop_style_var_.load());
  pop_style_color_ = 0;
  pop_style_var_ = 0;
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
#if IMGUI_GLUT_OPENGL2
 Window::Window(const IDearImGui* host) : OpenGL2GlutDrive(host, this) {
#elif IMGUI_GLFW_OPENGL2
 Window::Window(const IDearImGui * host) : OpenGL2GlfwDrive(host, this) {
#elif IMGUI_GLFW_OPENGL3
 Window::Window(const IDearImGui * host) : OpenGL3GlfwDrive(host, this) {
#elif IMGUI_WIN32_DIRECTX9
 Window::Window(const IDearImGui * host) : Directx9Drive(host, this) {
#elif IMGUI_WIN32_DIRECTX10
 Window::Window(const IDearImGui * host) : Directx10Drive(host, this) {
#elif IMGUI_WIN32_DIRECTX11
 Window::Window(const IDearImGui * host) : Directx11Drive(host, this) {
#elif IMGUI_WIN32_DIRECTX12
 Window::Window(const IDearImGui * host) : Directx12Drive(host, this) {
#endif
 }
 Window::~Window() {

 }
 void Window::Release() const {
  delete this;
 }
 void Window::OnRenderBegin() {
  do {
   if (!__super::Start())
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
      RECT rect = { 0, 0, static_cast<long>(size.GetCX()), static_cast<long>(size.GetCY()) };
      //::AdjustWindowRect(&rect, GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
      ::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
     }break;
     case AttributeType::bkcolor: {
      m_rgbaMainBkColor = ImVec4(style->GetVec4().GetR(), style->GetVec4().GetG(), style->GetVec4().GetB(), style->GetVec4().GetA());
      m_dwMainBkColor = style->GetVec4().GetARGB();
     }break;
     case AttributeType::visible: {
      const bool visible = style->GetBool();
      shared::Win::Window::Visible(m_hWnd, visible);
     }break;
     case AttributeType::center: {
      if (!style->GetBool())
       break;
      shared::Win::Window::Center(m_hWnd);
     }break;
     case AttributeType::minsize: {
      m_MinSize = style->GetVec2();
     }break;
     case AttributeType::maxsize: {
      m_MaxSize = style->GetVec2();
     }break;
     case AttributeType::maximize: {
      if (!style->GetBool())
       break;
      ::PostMessageW(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
     }break;
     case AttributeType::title_name: {
      if (!style->GetString())
       break;
      ::SetWindowTextW(m_hWnd, style->GetString()->wstr());
     }break;
     default:
      break;
     }
    });
  } while (0);
 }
 void Window::OnRenderEnd() {
  __super::Stop();
 }
 Vec2 Window::GetSize() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return m_Size;
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame() {

 }
 Frame::~Frame() {

 }
 void Frame::Release() const {
  delete this;
 }
 void Frame::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   if (!attribute_s_)
    break;

   const char* title = nullptr;
   attribute_s_->Iterator(
    [&](const AttributeType& type, IAttribute* style, bool& itbreak) {
     switch (type) {
     case AttributeType::alpha: {

     }break;
     case AttributeType::title_name: {
      title = reinterpret_cast<const char*>(style->GetString()->u8());
     }break;
     case AttributeType::bkcolor: {
      PushStyleCol(ImGuiCol_WindowBg, style->GetVec4());
     }break;
     case AttributeType::visible: {
      if (!style->GetBool())
       break;
      show_flags_ = true;
     }break;
     default:
      break;
     }
    });

   /*
   这些是ImGui的窗口标志（Window Flags），它们用于控制窗口的外观和行为。通过在创建窗口时设置这些标志，可以定制窗口的特性和功能。

   下面是对每个标志的解释：
   1. `ImGuiWindowFlags_NoTitleBar`: 禁用窗口的标题栏，使窗口没有标题。
   2. `ImGuiWindowFlags_NoScrollbar`: 禁用窗口的滚动条。如果窗口的内容超出了可见区域，将不会显示滚动条。
   3. `ImGuiWindowFlags_NoResize`: 禁止调整窗口的大小。窗口将无法通过拖动边框来改变大小。
   4. `ImGuiWindowFlags_NoMove`: 禁止移动窗口。窗口将无法通过拖动标题栏来移动。
   5. `ImGuiWindowFlags_NoCollapse`: 禁止窗口的折叠（最小化）功能。通常，窗口右上角会有一个折叠按钮，点击该按钮可以最小化窗口。
   6. `ImGuiWindowFlags_NoBringToFrontOnFocus`: 禁止在窗口获得焦点时将其置于最前面。通常，当窗口获得焦点时，它会自动移到其他窗口之上。
   7. `ImGuiWindowFlags_NoNavFocus`: 禁止窗口获得导航焦点。通常，通过键盘导航时，窗口会在不同的窗口之间切换焦点。
   8. `ImGuiWindowFlags_MenuBar`: 启用窗口的菜单栏。如果使用了这个标志，在窗口的顶部将显示一个菜单栏。
   9. `ImGuiWindowFlags_NoBackground`: 禁用窗口的背景绘制。窗口将没有背景颜色或背景图像。
   通过组合使用这些标志，你可以按需定制窗口的外观和行为。根据你的需求，可以选择使用适当的标志来创建满足特定要求的窗口。
   */
   PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(0, 0));
   PushStyleVar(ImGuiStyleVar_FramePadding, Vec2(0, 0));
   PushStyleVar(ImGuiStyleVar_ItemSpacing, Vec2(0, 0));
   PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, Vec2(0, 0));
   PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

   ImGui::SetNextWindowPos(ImVec2(layout_data_.x, layout_data_.y)); // 设置子控件的位置
   ImGui::SetNextWindowSize(ImVec2(layout_data_.z, layout_data_.w));
   if (!ImGui::Begin(title ? title : std::format("{:X}", reinterpret_cast<std::uintptr_t>(this)).c_str(),
    &show_flags_
    , ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoNavFocus
    /*| ImGuiWindowFlags_MenuBar*/
   ))
    break;

  } while (0);
 }
 void Frame::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
  PopStyle();
  ImGui::End();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 VerticalLayout::VerticalLayout() {

 }
 VerticalLayout::~VerticalLayout() {

 }
 void VerticalLayout::Release() const {
  delete this;
 }
 void VerticalLayout::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);

#if 0
  const IString* pName = nullptr;
  auto name = attribute_s_->GetAttribute(AttributeType::name);
  if (name)
   pName = name->GetString();
  if (pName)
   std::cout << std::format("{} size is ({},{},{},{})", pName->str(), layout_data_.x, layout_data_.y, layout_data_.z, layout_data_.w) << std::endl;
#endif

  PushStyleCol(ImGuiCol_FrameBg, attribute_s_->GetBkColor());
  ImGui::SetCursorPos(ImVec2(layout_data_.x, layout_data_.y));
  ImGui::BeginChildFrame(identify_, ImVec2(layout_data_.z, layout_data_.w));
 }
 void VerticalLayout::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
  PopStyle();
  ImGui::EndChildFrame();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 HorizontalLayout::HorizontalLayout() {

 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }
 void HorizontalLayout::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);


#if 0
  const IString* pName = nullptr;
  auto name = attribute_s_->GetAttribute(AttributeType::name);
  if (name)
   pName = name->GetString();
  if (pName)
   std::cout << std::format("{} size is {},{}", pName->str(), layout_data_.z, layout_data_.w) << std::endl;
#endif

  PushStyleCol(ImGuiCol_FrameBg, attribute_s_->GetBkColor());
  ImGui::SetCursorPos(ImVec2(layout_data_.x, layout_data_.y));
  ImGui::BeginChildFrame(identify_, ImVec2(layout_data_.z, layout_data_.w));
 }
 void HorizontalLayout::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
  PopStyle();
  ImGui::EndChildFrame();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 Button::Button() {

 }
 Button::~Button() {

 }
 void Button::Release() const {
  delete this;
 }
 void Button::OnRenderBegin() {
  std::lock_guard<std::mutex>(*m_Mutex);

  PushStyleCol(ImGuiCol_FrameBg, attribute_s_->GetBkColor());
  ImGui::SetCursorPos(ImVec2(layout_data_.x, layout_data_.y));
  ImGui::Button("Must have text", ImVec2(layout_data_.z, layout_data_.w));
 }
 void Button::OnRenderEnd() {
  std::lock_guard<std::mutex>(*m_Mutex);
  PopStyle();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////

}///namespace local


