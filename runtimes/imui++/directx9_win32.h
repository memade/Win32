#if !defined(__FE95D551_2A9C_4C47_830A_D5E1A190B5B8__)
#define __FE95D551_2A9C_4C47_830A_D5E1A190B5B8__

#if IMGUI_WIN32_DIRECTX9
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include <d3d9.h>
namespace local {

 class Directx9Drive final : public IDrive {
 public:
  Directx9Drive(const IDearImGui*);
  virtual ~Directx9Drive();
 public:
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  void Process() override final;
 private:
  WNDCLASSEXW m_WndClassEx = { 0 };
  std::atomic_bool m_IsOpen = false;
  LPDIRECT3D9 m_pD3D = nullptr;
  LPDIRECT3DDEVICE9 m_pd3dDevice = nullptr;
  D3DPRESENT_PARAMETERS m_d3dpp = { 0 };
 private:
  bool CreateDeviceD3D(HWND hWnd);
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
  void WaitForLastSubmittedFrame();
  void ResetDevice();
  static LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 };


}///namespace local
#pragma comment(lib,"d3d9.lib")
#endif

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:09:39 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__FE95D551_2A9C_4C47_830A_D5E1A190B5B8__
