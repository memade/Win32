#if !defined(__C82FC8AC_E08E_4D66_B561_71E6CBD6A909__)
#define __C82FC8AC_E08E_4D66_B561_71E6CBD6A909__

namespace local {

 class IDrive {
 protected:
  IDrive(const IDearImGui*);
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual void Release() const = 0;
  virtual void OnLoadFonts();
  virtual bool OnRender();
  virtual void OnRefreshBkcolor(DWORD&);
  virtual bool OnCreateApplicationWindow(IDrive* pCreateHost = nullptr,const WNDPROC& wndProc = nullptr);
  virtual void Process() = 0;
  virtual const HWND& Handle() const;
  const IDearImGui* Host() const { return m_pHost; }
 protected:
  HANDLE m_hMain = nullptr;
  HANDLE m_hCreate = nullptr;
  HWND m_hWnd = nullptr;
  std::vector<std::thread> m_Threads;
  const IDearImGui* m_pHost = nullptr;
 protected:
  WNDCLASSEX m_WndClassEx = { 0 };
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 03:14:19 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C82FC8AC_E08E_4D66_B561_71E6CBD6A909__