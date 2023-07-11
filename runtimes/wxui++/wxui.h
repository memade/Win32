#if !defined(__B986614D_5646_47EA_BF65_00A1C33CC211__)
#define __B986614D_5646_47EA_BF65_00A1C33CC211__

namespace local {

#if 0
 class Wxui final
  : public wxui::IWxui
  , public wxApp {
 public:
  Wxui();
  virtual ~Wxui();
 protected:
  void Release() const override final;
  bool Start() override final;
  void Stop() override final;
  void Show(const bool&) const override final;
  IFrame* FrameGet() const override final;
  HWND MainWnd() const override final;
  HWND CreateFrameChild(const bool& is_show) override final;
  HWND CreateFrameChildHost(const bool&) override final;
  HANDLE MainProcess() const override final;
  void RegisterOnAppCreateCb(const tfOnAppCreateCb&) override final;
  void RegisterOnDestroyCb(const tfOnDestroyCb&) override final;
  void MDIFrameWindowCascade() override final;
  void MDIFrameWindowTileHorz() override final;
  void MDIFrameWindowTileVert() override final;
  bool MDIAppendChild(const HWND&) override final;
  void MainWindowPos(const bool& auto_adjust /*= true*/, const RECT& rtPosition /*= { 0,0,0,0 }*/) override final;
 public:
  IConfig* ConfigGet() const override final;
  shared::IUIConfig* UIConfigGet() const override final;
 public:
  IFrame* CreateFrame(const FrameType&);
 private:
  void Init();
  void UnInit();
  void OnAppCreate(IApp* app) const;
  void OnDestroy() const;
  static unsigned int __stdcall MainThread(void*);
  HANDLE m_hMain = nullptr;
  std::atomic_bool m_IsOpen = false;
  Config* m_pConfig = nullptr;
  IFrame* m_pFrame = nullptr;
  tfOnAppCreateCb m_OnAppCreateCb = nullptr;
  tfOnDestroyCb m_OnDestroyCb = nullptr;
  /*std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();*/
 };
#endif


 class Wxui final : public IWxui {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Wxui();
  virtual ~Wxui();
 public:
  bool Start() override final;
  void Stop() override final;
  bool SkinCreate(const char*) override final;
  void SkinDestroy() override final;
  void Release() const override final;
  IControlUI* CreateControl(const ControlType&) override final;
 public:
  ISkin* SkinGet() const override final;
  shared::IUIConfig* UIConfigGet() const override final;
 private:
  void Init();
  void UnInit();
  static unsigned int __stdcall MainThread(void*);
  HANDLE m_hMain = nullptr;
  std::atomic_bool m_IsOpen = false;
  ISkin* m_pSkin = nullptr;
  shared::IUIConfig* m_pConfig = nullptr;
 };






 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B986614D_5646_47EA_BF65_00A1C33CC211__