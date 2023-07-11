#if !defined(__EB7FE262_6A8B_4F5C_B9CB_FAAE53710D70__)
#define __EB7FE262_6A8B_4F5C_B9CB_FAAE53710D70__

#if IMGUI_WIN32_DIRECTX11
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <d3d11.h>
namespace local {

 class Directx11Drive : public IDrive {
 public:
  Directx11Drive(const IDearImGui*, const Control*);
  virtual ~Directx11Drive();
 public:
  void Release() const override;
  void Process() override;
 protected:
  bool Create() override;
  void Destroy() override;
 private:
  ID3D11Device* m_pd3dDevice = nullptr;
  ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
  IDXGISwapChain* m_pSwapChain = nullptr;
  ID3D11RenderTargetView* m_mainRenderTargetView = nullptr;
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
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#endif

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:08:49 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__EB7FE262_6A8B_4F5C_B9CB_FAAE53710D70__

