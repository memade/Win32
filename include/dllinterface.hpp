#if !defined(__D43B5F42_3AE0_4B20_9C76_832BF5F3E267__)
#define __D43B5F42_3AE0_4B20_9C76_832BF5F3E267__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace shared {
 using t_interface_init = void* (__stdcall*)(const void*, unsigned long);
 using t_interface_uninit = void(__stdcall*)(void);

 template<typename T>
 class InterfaceDll {
 protected:
  InterfaceDll() {}
  ~InterfaceDll() {}
 public:
  static T* CreateInterface(const std::string& module_pathname, \
   const void* route = nullptr,
   const unsigned long& route_size = 0,
   const std::string& __api_object_init = "api_object_init", const std::string& __api_object_uninit = "api_object_uninit") {
   T* result = nullptr;
   HMODULE hModule = nullptr;
   do {
    if (module_pathname.empty())
     break;
    hModule = ::LoadLibraryA(module_pathname.c_str());
    if (!hModule)
     break;
    auto api_object_init__ = \
     reinterpret_cast<decltype(T::api_object_init)>(::GetProcAddress(hModule, __api_object_init.c_str()));
    auto api_object_uninit__ = \
     reinterpret_cast<decltype(T::api_object_uninit)>(::GetProcAddress(hModule, __api_object_uninit.c_str()));
    if (!api_object_init__ || !api_object_uninit__)
     break;
    result = reinterpret_cast<decltype(result)>(api_object_init__(route, route_size));
    if (!result)
     break;
    result->hModule = hModule;
    result->api_object_init = api_object_init__;
    result->api_object_uninit = api_object_uninit__;
   } while (0);
   if (nullptr == result && hModule != nullptr) {
    ::FreeLibrary(hModule);
    hModule = nullptr;
   }
   return result;
  }
  static void DestoryInterface(T*& instance) {
   do {
    if (!instance)
     break;
    if (!instance->hModule || !instance->api_object_uninit)
     break;
    HMODULE freeMod = reinterpret_cast<HMODULE>(instance->hModule);
    instance->api_object_uninit();
    instance->Release();
    instance = nullptr;
    ::FreeLibrary(freeMod);
    freeMod = nullptr;
   } while (0);
  }
 protected:
  void* hModule = nullptr;
  t_interface_init api_object_init = nullptr;
  t_interface_uninit api_object_uninit = nullptr;
 };


 class IUIConfig {
 public:
  virtual void Release() const {}
  virtual void MainWindowInitialShow(const bool&) { return; }
  virtual bool MainWindowInitialShow() const { return false; }
  virtual void RequestUrl(const char*) { return; }
  //!@ 子窗口独占模式
  virtual void EnableChildWindowExclusiveMode(const bool&) { return; }
  virtual bool EnableChildWindowExclusiveMode() const { return false; }
  virtual void EnableDpiAwareness(const bool&) {}
  virtual bool EnableDpiAwareness() const { return false; }
 };

 class IUserInterface {
 protected:
  IUserInterface* m_pUI = nullptr;
  using tfOnDestroyCb = std::function<void()>;
 public:
  virtual IUIConfig* UIConfigGet() const {
   return m_pUI ? m_pUI->UIConfigGet() : nullptr;
  }
  virtual bool Start() {
   return m_pUI ? m_pUI->Start() : false;
  }
  virtual void Stop() {
   if (m_pUI) return m_pUI->Stop();
  }
  virtual void Show(const bool& flag) const {
   if (m_pUI) m_pUI->Show(flag);
  }
  virtual void Release() const { return; }
  virtual void Parent(const HWND& h) {
   if (m_pUI) m_pUI->Parent(h);
  }
  virtual HWND MainWnd() const {
   return m_pUI ? m_pUI->MainWnd() : nullptr;
  }
  virtual HANDLE MainProcess() const {
   return m_pUI ? m_pUI->MainProcess() : nullptr;
  }
  virtual HWND CreateFrameChild(const bool& show_flag) {
   return m_pUI ? m_pUI->CreateFrameChild(show_flag) : nullptr;
  }
  virtual HWND CreateFrameChildHost(const bool& show_flag) {
   return m_pUI ? m_pUI->CreateFrameChildHost(show_flag) : nullptr;
  }
  virtual void RegisterOnDestroyCb(const tfOnDestroyCb& cb) {
   if (m_pUI)
    m_pUI->RegisterOnDestroyCb(cb);
  }
  virtual void MDIFrameWindowCascade() {
   if (m_pUI)
    m_pUI->MDIFrameWindowCascade();
  }
  virtual void MDIFrameWindowTileHorz() {
   if (m_pUI)
    m_pUI->MDIFrameWindowTileHorz();
  }
  virtual void MDIFrameWindowTileVert() {
   if (m_pUI)
    m_pUI->MDIFrameWindowTileVert();
  }
  virtual bool MDIAppendChild(const HWND& h) {
   return m_pUI ? m_pUI->MDIAppendChild(h) : false;
  }
  virtual void MainWindowPos(const bool& auto_adjust = true, const RECT& rtPosition = { 0,0,0,0 }) {
   if (m_pUI)
    m_pUI->MainWindowPos(auto_adjust, rtPosition);
  }
 };

}///namespace shared



/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Dec 2022 05:38:48 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__D43B5F42_3AE0_4B20_9C76_832BF5F3E267__