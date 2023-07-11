#if !defined(__CC3E7B38_9189_4884_BABE_D1267B8B408C__)
#define __CC3E7B38_9189_4884_BABE_D1267B8B408C__

namespace local {

 class Control : public skin::IControlUI {
 public:
  Control();
  virtual ~Control();
 protected:
  virtual void PopStyle();
  virtual void PushStyleCol(const ImGuiCol_&,const Vec4&);
  virtual void PushStyleVar(const ImGuiStyleVar&, const Vec2&);
  virtual void PushStyleVar(const ImGuiStyleVar&, const TypePixels&);
  void OnLayout() override {}
  Vec2 GetSize() const override { return Vec2(); }
 protected:
  HICON logo_icon_ = nullptr;
  bool show_flags_ = false;
  std::atomic_ulong  pop_style_var_ = 0;
  std::atomic_ulong  pop_style_color_ = 0;
 };

#if IMGUI_GLUT_OPENGL2
 class Window final : public Control, public OpenGL2GlutDrive {
#elif IMGUI_GLFW_OPENGL2
 class Window final : public Control, public OpenGL2GlfwDrive {
#elif IMGUI_GLFW_OPENGL3
 class Window final : public Control, public OpenGL3GlfwDrive {
#elif IMGUI_WIN32_DIRECTX9
 class Window final : public Control, public Directx9Drive {
#elif IMGUI_WIN32_DIRECTX10
 class Window final : public Control, public Directx10Drive {
#elif IMGUI_WIN32_DIRECTX11
 class Window final : public Control, public Directx11Drive {
#elif IMGUI_WIN32_DIRECTX12
 class Window final : public Control, public Directx12Drive {
#endif
 public:
  Window(const IDearImGui*);
  virtual ~Window();
  void Release() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
 protected:
  Vec2 GetSize() const override final;
 };

 class Frame final : public Control {
 public:
  Frame();
  virtual ~Frame();
  void Release() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
 };

 class VerticalLayout final : public Control {
 public:
  VerticalLayout();
  virtual ~VerticalLayout();
  void Release() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
 };

 class HorizontalLayout final : public Control {
 public:
  HorizontalLayout();
  virtual ~HorizontalLayout();
  void Release() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
 };

 class Button final : public Control {
 public:
  Button();
  virtual ~Button();
  void Release() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
 };

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 28 Jun 2023 00:01:37 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__CC3E7B38_9189_4884_BABE_D1267B8B408C__