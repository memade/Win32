#if !defined(__B986614D_5646_47EA_BF65_00A1C33CC211__)
#define __B986614D_5646_47EA_BF65_00A1C33CC211__

namespace local {

 using tfRenderCb = std::function<void(void)>;
 using tfImguiErrorCb = std::function<void(const int&, const char*)>;

 class Imui : public IImui {
 public:
  Imui();
  virtual ~Imui();
 protected:
  void Release() const override final;
  IConfig* ConfigGet() const override final;
  shared::IUIConfig* UIConfigGet() const override final;
  bool Start() override final;
  void Stop() override final;
  void Show(const bool&) const override final;
  HWND MainWnd() const override final;
  HANDLE MainProcess() const override final;
  HWND CreateFrameChild(const bool&) override final { return nullptr; }
  HWND CreateFrameChildHost(const bool&) override final { return nullptr; }
  void RegisterRenderCb(const tfRenderCb&);
  void RegisterRenderCb(const tfImguiErrorCb&);
  void RegisterOnDestroyCb(const tfOnDestroyCb&) override final;
  void RegisterOnCreateWindowCb(const tfOnCreateWindowCb&) override final;
  void RegisterOnCreateWindowClassCb(const tfOnCreateWindowClassCb&) override final;
 private:
  void Init();
  void UnInit();
  void Render();
  void OnDestroy() const;
  void OnCreateWindow(
   DWORD& dwExStyle,
   LPCWSTR& lpClassName,
   LPCWSTR& lpWindowName,
   DWORD& dwStyle,
   int& X,
   int& Y,
   int& nWidth,
   int& nHeight,
   HWND& hWndParent,
   HMENU& hMenu,
   HINSTANCE& hInstance,
   LPVOID& lpParam);
  void OnCreateWindowClass(PWNDCLASSEXW);
  Config* m_pConfig = nullptr;
#if IMGUI_WIN32_DIRECTX12 || IMGUI_WIN32_DIRECTX9 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX10
  bool CreateDeviceD3D(HWND hWnd);
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
  void WaitForLastSubmittedFrame();
  void ResetDevice();
#if IMGUI_WIN32_DIRECTX12
  FrameContext* WaitForNextFrameResources();
#endif
  static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
  static unsigned int __stdcall MainProcess(void*);
 protected:
  HANDLE m_hMain = nullptr;
  HWND m_hWnd = nullptr;
  SIZE m_WindowSize = { 1280,768 };
  ImGuiIO* m_pImGuiIO = nullptr;
  std::atomic_bool m_Ready = false;
  std::atomic_bool m_IsOpen = false;
  tfRenderCb m_RenderCb = nullptr;
  tfImguiErrorCb m_ErrorCb = nullptr;
  tfOnDestroyCb m_OnDestroyCb = nullptr;
  tfOnCreateWindowCb m_OnCreateWindowCb = nullptr;
  tfOnCreateWindowClassCb m_OnCreateWindowClassCb = nullptr;
  std::thread m_RenderThread;
#if IMGUI_GLFW_OPENGL2 || IMGUI_GLFW_OPENGL3
  GLFWwindow* m_pGLFWwindow = nullptr;
#elif IMGUI_WIN32_DIRECTX9
  LPDIRECT3D9 m_pD3D = nullptr;
  LPDIRECT3DDEVICE9 m_pd3dDevice = nullptr;
  D3DPRESENT_PARAMETERS m_d3dpp = { 0 };
#elif IMGUI_WIN32_DIRECTX10
  ID3D10Device* m_pd3dDevice = nullptr;
  IDXGISwapChain* m_pSwapChain = nullptr;
  ID3D10RenderTargetView* m_mainRenderTargetView = nullptr;
#elif IMGUI_WIN32_DIRECTX11
  ID3D11Device* m_pd3dDevice = nullptr;
  ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
  IDXGISwapChain* m_pSwapChain = nullptr;
  ID3D11RenderTargetView* m_mainRenderTargetView = nullptr;
#elif IMGUI_WIN32_DIRECTX12
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
#endif
 };


 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B986614D_5646_47EA_BF65_00A1C33CC211__