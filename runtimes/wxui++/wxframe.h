#if !defined(__D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__)
#define __D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__

namespace local {

#if 0
 class Frame final
  : public wxFrame
  , public IFrame {
 public:
  Frame(
   const Wxui* host,
   wxWindow* parent = nullptr,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(Frame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxDEFAULT_FRAME_STYLE,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~Frame();
 public:
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  IChildFrame* CreateChildFrame() override final { return nullptr; }
  IChildFrame* CreateChildFrameHost(const bool&) override final { return nullptr; }
 protected:
  void RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb&) override final { return; }
  void RegisterOnFrameCreateChildHostCb(const tfOnFrameCreateChildHostCb&) override final { return; }
 private:
  const Wxui* m_pWxui;
 };
 class MDIFrame final
  : public wxMDIParentFrame
  , public IFrame {
 public:
  MDIFrame(
   const Wxui* host,
   wxWindow* parent = nullptr,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(MDIFrame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = /*wxDefaultSize*/wxSize(1024, 768),
   long style = wxDEFAULT_FRAME_STYLE | wxFRAME_NO_WINDOW_MENU /*wxVSCROLL | wxHSCROLL*/,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~MDIFrame();
 public:
  const Wxui* Host() const;
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  IChildFrame* CreateChildFrame() override final;
  IChildFrame* CreateChildFrameHost(const bool&) override final;
  void MDIWindowCascade() override final;
  void MDIWindowTileHorz() override final;
  void MDIWindowTileVert() override final;
  bool MDIAppendChild(const HWND&) override final;
 protected:
  tfOnFrameCreateChildCb m_OnFrameCreateChildCb = nullptr;
  tfOnFrameCreateChildHostCb m_OnFrameCreateChildHostCb = nullptr;
  void RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb&) override final;
  void RegisterOnFrameCreateChildHostCb(const tfOnFrameCreateChildHostCb&) override final;
 private:
  void OnFrameCreateChild(IChildFrame* child) const;
  void OnFrameCreateChildHost(IChildFrame* child) const;
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
 private:
  const Wxui* m_pWxui;
 };
#endif

 class Frame final : public wxFrame, public IFrame {
 public:
  Frame(const IWxui* host,
   wxWindow* parent = nullptr,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(Frame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxDEFAULT_FRAME_STYLE,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~Frame();
 protected:
  void Show(const bool&) override final;
  void Size(const ISize*) override final;
  void Pos(const IPos*) override final;
  void Align(const IAlign*) override final;
  void* Handle() const override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
  bool show_ = false;
  const IWxui* host_;
 };


}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 02:26:56 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__