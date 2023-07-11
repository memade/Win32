#if !defined(__B986614D_5646_47EA_BF65_00A1C33CC211__)
#define __B986614D_5646_47EA_BF65_00A1C33CC211__

namespace local {

 class DearImGui : public IDearImGui {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  DearImGui();
  virtual ~DearImGui();
 protected:
  void Release() const override final;
  bool SkinCreate(const char*) override final;
  void SkinDestroy() override final;
  class ISkin* SkinGet() const override final;
  bool Start() override final;
  void Stop() override final;
  IControlUI* CreateControl(const ControlType&) override final;
 public:
  shared::IUIConfig* UIConfigGet() const override final;
  void NotifyUICreateSuccess() const;
 private:
  ISkin* m_pSkin = nullptr;
  shared::IUIConfig* m_pConfig = nullptr;
 public:
  void Init();
  void UnInit();
  static unsigned int __stdcall MainProcess(void*);
 public:
  HANDLE m_hMain = nullptr;
  HANDLE m_hEventCreateUI = nullptr;
  HWND m_hWnd = nullptr;
  SIZE m_WindowSize = { 1280,768 };
  ImGuiIO* m_pImGuiIO = nullptr;
  std::atomic_bool m_Ready = false;
  std::atomic_bool m_IsOpen = false;
 };


 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B986614D_5646_47EA_BF65_00A1C33CC211__