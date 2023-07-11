#if !defined(__C82FC8AC_E08E_4D66_B561_71E6CBD6A909__)
#define __C82FC8AC_E08E_4D66_B561_71E6CBD6A909__

namespace local {
 class IDrive {
 protected:
  IDrive(const IDearImGui*, const class Control*);
  ~IDrive();
 public:
  virtual bool Start();
  virtual void Stop();
  virtual void Release() const = 0;
  virtual const HWND& Hwnd() const;
  virtual void SetSize(const Vec2&);
 protected:
  virtual void CreateWait() const;
  virtual void CreateNotify() const;
  virtual void OnLoadFonts();
  virtual bool OnRender();
  virtual void OnLayout();
  virtual void Process() = 0;
  virtual bool Create() = 0;
  virtual void Destroy() = 0;
 protected:
  Vec2 m_MaxSize;
  Vec2 m_MinSize;
  Vec2 m_Size;
  const class Control* m_pControl;
  const IDearImGui* m_pHost;
  HANDLE m_hMain = nullptr;
  HANDLE m_hCreate = nullptr;
  HWND m_hWnd = nullptr;
  std::vector<std::thread> m_Threads;
  std::atomic_bool m_IsOpen = false;
  DWORD m_dwMainBkColor = 0;
  ImVec4 m_rgbaMainBkColor;
#if IMGUI_DIRECTX_ENABLE
  WNDCLASSEXW m_WndClassEx = { 0 };
  CREATESTRUCTW m_WndCreateStructW = { 0 };
#endif
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 03:14:19 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C82FC8AC_E08E_4D66_B561_71E6CBD6A909__