#if !defined(__E2498732_403D_43F2_B5DD_B5587D20D955__)
#define __E2498732_403D_43F2_B5DD_B5587D20D955__

#if IMGUI_WIN32_DIRECTX12
#define NUM_FRAMES_IN_FLIGHT 3
#define NUM_BACK_BUFFERS 3
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
typedef struct tagFrameContext
{
 ID3D12CommandAllocator* CommandAllocator;
 UINT64 FenceValue;
 tagFrameContext() { memset(this, 0x00, sizeof(*this)); }
 void operator=(const tagFrameContext& obj) { memcpy(this, &obj, sizeof(*this)); }
}FrameContext;
namespace local {

 class Directx12Drive : public IDrive {
 public:
  Directx12Drive(const IDearImGui*, const Control*);
  virtual ~Directx12Drive();
 public:
  void Release() const override;
  void Process() override;
 protected:
  bool Create() override;
  void Destroy() override;
 private:
  UINT m_nFrameIndex = 0;
  UINT64 m_nFenceLastSignaledValue = 0;
  ID3D12Device* m_pD3D12Device = nullptr;
  ID3D12DescriptorHeap* m_pD3DRtvDescHeap = nullptr;
  ID3D12DescriptorHeap* m_pD3DSrvDescHeap = nullptr;
  ID3D12CommandQueue* m_pD3DCommandQueue = nullptr;
  ID3D12GraphicsCommandList* m_pD3DCommandList = nullptr;
  ID3D12Fence* m_pFence = nullptr;
  HANDLE m_hFenceEvent = nullptr;
  IDXGISwapChain3* m_pSwapChain = nullptr;
  HANDLE m_hSwapChainWaitableObject = nullptr;
  FrameContext m_FrameContexts[NUM_FRAMES_IN_FLIGHT];
  ID3D12Resource* m_pMainRenderTargetResources[NUM_BACK_BUFFERS] = { 0 };
  D3D12_CPU_DESCRIPTOR_HANDLE  m_pMainRenderTargetDescriptors[NUM_BACK_BUFFERS] = { 0 };
 private:
  bool CreateDeviceD3D(HWND hWnd);
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
  void WaitForLastSubmittedFrame();
  void ResetDevice();
  FrameContext* WaitForNextFrameResources();
  static LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 };

}///namespace local
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#if _DEBUG
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#endif

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:05:16 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__E2498732_403D_43F2_B5DD_B5587D20D955__

