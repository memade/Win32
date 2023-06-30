#if !defined(__6FD73C03_A932_409B_9047_46D2782AEBC8__)
#define __6FD73C03_A932_409B_9047_46D2782AEBC8__

#if IMGUI_WIN32_DIRECTX10
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_win32.h>
#include <d3d10_1.h>
#include <d3d10.h>
namespace local {

 class Directx10Drive final : public IDrive {
 public:
  Directx10Drive(const IDearImGui*);
  virtual ~Directx10Drive();
 public:
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  void Process() override final;
 private:
  WNDCLASSEXW m_WndClassEx = { 0 };
  std::atomic_bool m_IsOpen = false;
  ID3D10Device* m_pd3dDevice = nullptr;
  IDXGISwapChain* m_pSwapChain = nullptr;
  ID3D10RenderTargetView* m_mainRenderTargetView = nullptr;
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
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#endif


/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:09:09 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6FD73C03_A932_409B_9047_46D2782AEBC8__

