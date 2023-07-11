#if !defined(__1A0DB085_4C94_4235_B700_62A77727CEAA__)
#define __1A0DB085_4C94_4235_B700_62A77727CEAA__

namespace local {

 class Control : public skin::IControlUI {
 public:
  Control();
  virtual ~Control();
 };

 class Window final : public Control, public wxMDIParentFrame {
 public:
  Window(const class Wxui*);
  virtual ~Window();
 public:
  void Release() const override final;
 protected:
  Vec2 GetSize() const override final;
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
  void OnLayout() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
 private:
  const class Wxui* host_;
  HICON logo_icon_ = nullptr;
 };

 class Frame final : public Control, public wxMDIChildFrame {
 public:
  Frame();
  virtual ~Frame();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
  void OnLayout() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
 };


 class VerticalLayout final : public Control, public wxPanel {
 public:
  VerticalLayout();
  virtual ~VerticalLayout();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
  void OnLayout() override final;
 private:
  void OnSize(wxSizeEvent& wxEvent);
 };

 class HorizontalLayout final : public Control, public wxPanel {
 public:
  HorizontalLayout();
  virtual ~HorizontalLayout();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
  void OnLayout() override final;
 private:
  void OnSize(wxSizeEvent& wxEvent);
 };

 class Button final : public Control, public wxButton {
 public:
  Button();
  virtual ~Button();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin() override final;
  void OnRenderEnd() override final;
  void OnLayout() override final;
 private:
  void OnSize(wxSizeEvent& wxEvent);
 };

#if 0
 class Node : public INodeUI {
 public:
  Node();
  ~Node();
 protected:
  void NodeSK(INode*) override;
  void NodeSK(INodeSK*) override;
  INodeSK* NodeSK() const override;
  void Parent(INodeUI*) override;
  INodeUI* Parent() const override;
 protected:
  void Refresh() override;
  void SetTitle(const IString*) {}
  void Center() override;
  void SetBkColor(const Vec4&) override;
  void Show(const bool&) override;
  void SetSize(const Vec2&) override;
  void SetLogo(const HICON&) override;
  void SetMaxSize(const Vec2&) override;
  void SetMinSize(const Vec2&) override;
  void Maximize() override;
  void Restore() override;
  void SetRounding(const TypePixelsType&) override {}
  void SetBorderSize(const TypePixelsType&) override {}
  void SetBorderColor(const Vec4&) override {}
 protected:
  bool flag_open_ = false;
 public:
  virtual void OnLayout() const;
 };

 class Window final : public Node, public wxMDIParentFrame {
 public:
  Window();
  virtual ~Window();
 public:
  void Release() const override final;
 protected:
  void Maximize() override final;
  void Restore() override final;
  void SetTitle(const IString*) override final;
  void OnRenderBegin(const tfOnRenderStyleCb&) override final;
  void OnRenderEnd() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
 };

 class Frame final : public Node, public wxMDIChildFrame {
 public:
  Frame();
  virtual ~Frame();
 public:
  void Release() const override final;
 protected:
  void Maximize() override final;
  void Restore() override final;
  void SetTitle(const IString*) override final;
  void OnRenderBegin(const tfOnRenderStyleCb&) override final;
  void OnRenderEnd() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
 };


 class VerticalLayout final : public Node, public wxPanel {
 public:
  VerticalLayout();
  virtual ~VerticalLayout();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin(const tfOnRenderStyleCb&) override final;
  void OnRenderEnd() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
 };

 class HorizontalLayout final : public Node, public wxPanel {
 public:
  HorizontalLayout();
  virtual ~HorizontalLayout();
 public:
  void Release() const override final;
 protected:
  void OnRenderBegin(const tfOnRenderStyleCb&) override final;
  void OnRenderEnd() override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
 };







 class TreeList final : public wxTreeListCtrl {
 public:
  TreeList(wxWindow* parent,
   wxWindowID id = wxID_ANY,
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxTL_DEFAULT_STYLE,
   const wxString& name = wxASCII_STR(wxTreeListCtrlNameStr));
  virtual ~TreeList();
 private:
  void Init();
  void UnInit();
 };

#endif

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 24 Jun 2023 01:17:44 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__1A0DB085_4C94_4235_B700_62A77727CEAA__