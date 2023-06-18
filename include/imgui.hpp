#if !defined(__0A8B1668_73C2_46DD_BD4A_5A35A7A176C0__)
#define __0A8B1668_73C2_46DD_BD4A_5A35A7A176C0__

#define IMGUI_GLUT_OPENGL2 0
#define IMGUI_GLFW_OPENGL3 0
#define IMGUI_GLFW_OPENGL2 0
#define IMGUI_WIN32_DIRECTX9 0
#define IMGUI_WIN32_DIRECTX10 0
#define IMGUI_WIN32_DIRECTX11 0
#define IMGUI_WIN32_DIRECTX12 1
#include <imgui.h>

#if IMGUI_GLUT_OPENGL2
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl2.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#elif IMGUI_GLFW_OPENGL2
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
#elif IMGUI_GLFW_OPENGL3
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#elif IMGUI_WIN32_DIRECTX9
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#elif IMGUI_WIN32_DIRECTX10
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_win32.h>
#include <d3d10_1.h>
#include <d3d10.h>
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#elif IMGUI_WIN32_DIRECTX11
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#elif IMGUI_WIN32_DIRECTX12
#define NUM_FRAMES_IN_FLIGHT 3
#define NUM_BACK_BUFFERS 3
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#if _DEBUG
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
typedef struct tagFrameContext
{
 ID3D12CommandAllocator* CommandAllocator;
 UINT64 FenceValue;
 tagFrameContext() { memset(this, 0x00, sizeof(*this)); }
 void operator=(const tagFrameContext& obj) { memcpy(this, &obj, sizeof(*this)); }
}FrameContext;
#endif

#if IMGUI_WIN32_DIRECTX12 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX10 || IMGUI_WIN32_DIRECTX9
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace ImGui {
 // Our state
 static bool show_demo_window = true;
 static bool show_another_window = false;
 static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 using tfRenderCb = std::function<void(void)>;
 using tfImguiErrorCb = std::function<void(const int&, const char*)>;

 class ImguiBase {
 public:
  ImguiBase(const HWND& parent = nullptr)
   :m_hParent(parent) {
  }
  ~ImguiBase() {
  }
  void Release() const { delete this; }
 public:
  bool Start() {
   do {
    if (m_IsOpen.load())
     break;
    m_hWaitForMainWndCreate = ::CreateEventW(NULL, TRUE, FALSE, NULL);
    m_hMainThread = reinterpret_cast<HANDLE>(::_beginthreadex(nullptr, 0, MainProcess, this, 0, nullptr));
    if (m_hWaitForMainWndCreate) {
     ::WaitForSingleObject(m_hWaitForMainWndCreate, INFINITE);
     ::CloseHandle(m_hWaitForMainWndCreate);
     m_hWaitForMainWndCreate = nullptr;
    }
    m_IsOpen.store(true);
   } while (0);
   return m_IsOpen.load();
  }
  void Stop() {
   do {
    if (!m_IsOpen.load())
     break;
    m_IsOpen.store(false);
    ::SendMessageW(m_hWnd, WM_CLOSE, NULL, NULL);
    if (m_hMainThread) {
     ::WaitForSingleObject(m_hMainThread, INFINITE);
     ::CloseHandle(m_hMainThread);
     m_hMainThread = nullptr;
    }



#if IMGUI_WIN32_DIRECTX9

#elif IMGUI_WIN32_DIRECTX10

#elif IMGUI_WIN32_DIRECTX11

#elif IMGUI_WIN32_DIRECTX12

#elif IMGUI_GLUT_OPENGL2
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
    m_RenderThread.join();
#elif IMGUI_GLFW_OPENGL2

#elif IMGUI_GLFW_OPENGL3

#endif
   } while (0);
  }
  const HANDLE& MainHandle() const {
   return m_hMainThread;
  }
  const HWND& Hwnd() const {
   return m_hWnd;
  }
  void RegisterRenderCb(const tfRenderCb& render_cb) {
   m_RenderCb = render_cb;
  }
  void RegisterRenderCb(const tfImguiErrorCb& error_cb) {
   m_ErrorCb = error_cb;
  }
 private:
  void Render() {

  }

#if IMGUI_WIN32_DIRECTX12 || IMGUI_WIN32_DIRECTX9 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX10
  inline bool CreateDeviceD3D(HWND hWnd);
  inline void CleanupDeviceD3D();
  inline void CreateRenderTarget();
  inline void CleanupRenderTarget();
  inline void WaitForLastSubmittedFrame();
  inline void ResetDevice();
#if IMGUI_WIN32_DIRECTX12
  inline FrameContext* WaitForNextFrameResources();
#endif
  inline static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
  inline static unsigned int __stdcall MainProcess(void*);
 protected:
  HANDLE m_hMainThread = nullptr;
  HANDLE m_hWaitForMainWndCreate = nullptr;
  HWND m_hWnd = nullptr;
  const HWND m_hParent;
  SIZE m_WindowSize = { 1280,768 };
  ImGuiIO* m_pImGuiIO = nullptr;
  std::atomic_bool m_Ready = false;
  std::atomic_bool m_IsOpen = false;
  tfRenderCb m_RenderCb = nullptr;
  tfImguiErrorCb m_ErrorCb = nullptr;
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
#if IMGUI_GLFW_OPENGL3


#endif

#if IMGUI_GLFW_OPENGL2

#endif


#if IMGUI_WIN32_DIRECTX12 || IMGUI_WIN32_DIRECTX9 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX10

 inline bool ImguiBase::CreateDeviceD3D(HWND hWnd) {
  bool result = false;
  do {
#if IMGUI_WIN32_DIRECTX9
   if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
    return false;
   // Create the D3DDevice
   ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
   m_d3dpp.Windowed = TRUE;
   m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
   m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
   m_d3dpp.EnableAutoDepthStencil = TRUE;
   m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
   m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
   //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
   if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice) < 0)
    break;
#elif IMGUI_WIN32_DIRECTX10
   // Setup swap chain
   DXGI_SWAP_CHAIN_DESC sd;
   ZeroMemory(&sd, sizeof(sd));
   sd.BufferCount = 2;
   sd.BufferDesc.Width = 0;
   sd.BufferDesc.Height = 0;
   sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   sd.BufferDesc.RefreshRate.Numerator = 60;
   sd.BufferDesc.RefreshRate.Denominator = 1;
   sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
   sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd.OutputWindow = hWnd;
   sd.SampleDesc.Count = 1;
   sd.SampleDesc.Quality = 0;
   sd.Windowed = TRUE;
   sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   UINT createDeviceFlags = 0;
   //createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
   HRESULT res = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice);
   if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
    res = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_WARP, NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice);
   if (res != S_OK)
    break;
   CreateRenderTarget();
#elif IMGUI_WIN32_DIRECTX11
   // Setup swap chain
   DXGI_SWAP_CHAIN_DESC sd;
   ZeroMemory(&sd, sizeof(sd));
   sd.BufferCount = 2;
   sd.BufferDesc.Width = 0;
   sd.BufferDesc.Height = 0;
   sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   sd.BufferDesc.RefreshRate.Numerator = 60;
   sd.BufferDesc.RefreshRate.Denominator = 1;
   sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
   sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd.OutputWindow = hWnd;
   sd.SampleDesc.Count = 1;
   sd.SampleDesc.Quality = 0;
   sd.Windowed = TRUE;
   sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   UINT createDeviceFlags = 0;
   //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
   D3D_FEATURE_LEVEL featureLevel;
   const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
   HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
   if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
    res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
   if (res != S_OK)
    break;
   CreateRenderTarget();
#elif IMGUI_WIN32_DIRECTX12
   // Setup swap chain
   DXGI_SWAP_CHAIN_DESC1 sd;
   {
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = NUM_BACK_BUFFERS;
    sd.Width = 0;
    sd.Height = 0;
    sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    sd.Scaling = DXGI_SCALING_STRETCH;
    sd.Stereo = FALSE;
   }
   // [DEBUG] Enable debug interface
#if _DEBUG
   ID3D12Debug* pdx12Debug = NULL;
   if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
    pdx12Debug->EnableDebugLayer();
#endif
   // Create device
   D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
   if (D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&m_pD3D12Device)) != S_OK)
    break;
   // [DEBUG] Setup debug interface to break on any warnings/errors
#if _DEBUG
   if (pdx12Debug != NULL)
   {
    ID3D12InfoQueue* pInfoQueue = NULL;
    m_pD3D12Device->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
    pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
    pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
    pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
    pInfoQueue->Release();
    pdx12Debug->Release();
   }
#endif
   {
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.NumDescriptors = NUM_BACK_BUFFERS;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 1;
    if (m_pD3D12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pD3DRtvDescHeap)) != S_OK)
     break;
    SIZE_T rtvDescriptorSize = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pD3DRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++) {
     m_pMainRenderTargetDescriptors[i] = rtvHandle;
     rtvHandle.ptr += rtvDescriptorSize;
    }
   }
   {
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (m_pD3D12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pD3DSrvDescHeap)) != S_OK)
     break;
   }
   {
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 1;
    if (m_pD3D12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_pD3DCommandQueue)) != S_OK)
     break;
   }
   bool bCreateCommandAllocator = true;
   for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++) {
    if (m_pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_FrameContexts[i].CommandAllocator)) != S_OK) {
     bCreateCommandAllocator = false;
     break;
    }
   }
   if (!bCreateCommandAllocator)
    break;
   if (m_pD3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_FrameContexts[0].CommandAllocator, NULL, IID_PPV_ARGS(&m_pD3DCommandList)) != S_OK ||
    m_pD3DCommandList->Close() != S_OK)
    break;
   if (m_pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)) != S_OK)
    break;
   m_hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
   if (m_hFenceEvent == NULL)
    break;
   {
    IDXGIFactory4* dxgiFactory = NULL;
    IDXGISwapChain1* swapChain1 = NULL;
    if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
     break;
    if (dxgiFactory->CreateSwapChainForHwnd(m_pD3DCommandQueue, hWnd, &sd, NULL, NULL, &swapChain1) != S_OK)
     break;
    if (swapChain1->QueryInterface(IID_PPV_ARGS(&m_pSwapChain)) != S_OK)
     break;
    swapChain1->Release();
    dxgiFactory->Release();
    m_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
    m_hSwapChainWaitableObject = m_pSwapChain->GetFrameLatencyWaitableObject();
   }
   CreateRenderTarget();
#endif
   result = true;
  } while (0);
  return result;
 }
 inline void ImguiBase::ResetDevice() {
#if IMGUI_WIN32_DIRECTX9
  ImGui_ImplDX9_InvalidateDeviceObjects();
  HRESULT hr = m_pd3dDevice->Reset(&m_d3dpp);
  if (hr == D3DERR_INVALIDCALL)
   IM_ASSERT(0);
  ImGui_ImplDX9_CreateDeviceObjects();
#endif
 }
 inline void ImguiBase::CleanupDeviceD3D() {
#if IMGUI_WIN32_DIRECTX9
  if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
  if (m_pD3D) { m_pD3D->Release(); m_pD3D = NULL; }
#elif IMGUI_WIN32_DIRECTX10
  CleanupRenderTarget();
  if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
  if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
#elif IMGUI_WIN32_DIRECTX11
  CleanupRenderTarget();
  if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
  if (m_pd3dDeviceContext) { m_pd3dDeviceContext->Release(); m_pd3dDeviceContext = NULL; }
  if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
#elif IMGUI_WIN32_DIRECTX12
  CleanupRenderTarget();
  if (m_pSwapChain) { m_pSwapChain->SetFullscreenState(false, NULL); m_pSwapChain->Release(); m_pSwapChain = NULL; }
  if (m_hSwapChainWaitableObject != NULL) { CloseHandle(m_hSwapChainWaitableObject); }
  for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
   if (m_FrameContexts[i].CommandAllocator) { m_FrameContexts[i].CommandAllocator->Release(); m_FrameContexts[i].CommandAllocator = NULL; }
  if (m_pD3DCommandQueue) { m_pD3DCommandQueue->Release(); m_pD3DCommandQueue = NULL; }
  if (m_pD3DCommandList) { m_pD3DCommandList->Release(); m_pD3DCommandList = NULL; }
  if (m_pD3DRtvDescHeap) { m_pD3DRtvDescHeap->Release(); m_pD3DRtvDescHeap = NULL; }
  if (m_pD3DSrvDescHeap) { m_pD3DSrvDescHeap->Release(); m_pD3DSrvDescHeap = NULL; }
  if (m_pFence) { m_pFence->Release(); m_pFence = NULL; }
  if (m_hFenceEvent) { CloseHandle(m_hFenceEvent); m_hFenceEvent = NULL; }
  if (m_pD3D12Device) { m_pD3D12Device->Release(); m_pD3D12Device = NULL; }
#if _DEBUG
  IDXGIDebug1* pDebug = NULL;
  if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug)))) {
   pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
   pDebug->Release();
  }
#endif
#endif
 }
 inline void ImguiBase::CreateRenderTarget() {
#if IMGUI_WIN32_DIRECTX9

#elif IMGUI_WIN32_DIRECTX10
  ID3D10Texture2D* pBackBuffer = nullptr;
  do {
   m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
   if (!pBackBuffer)
    break;
   m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
   pBackBuffer->Release();
  } while (0);
#elif IMGUI_WIN32_DIRECTX11
  ID3D11Texture2D* pBackBuffer = nullptr;
  do {
   m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
   if (!pBackBuffer)
    break;
   m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
   pBackBuffer->Release();
  } while (0);
#elif IMGUI_WIN32_DIRECTX12
  for (UINT i = 0; i < NUM_BACK_BUFFERS; i++) {
   ID3D12Resource* pBackBuffer = NULL;
   m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
   m_pD3D12Device->CreateRenderTargetView(pBackBuffer, NULL, m_pMainRenderTargetDescriptors[i]);
   m_pMainRenderTargetResources[i] = pBackBuffer;
  }
#endif
 }
 inline void ImguiBase::CleanupRenderTarget() {
#if IMGUI_WIN32_DIRECTX9

#elif IMGUI_WIN32_DIRECTX10
  if (m_mainRenderTargetView) {
   m_mainRenderTargetView->Release();
   m_mainRenderTargetView = NULL;
  }
#elif IMGUI_WIN32_DIRECTX11
  if (m_mainRenderTargetView) { m_mainRenderTargetView->Release(); m_mainRenderTargetView = NULL; }
#elif IMGUI_WIN32_DIRECTX12
  WaitForLastSubmittedFrame();
  for (UINT i = 0; i < NUM_BACK_BUFFERS; i++) {
   if (m_pMainRenderTargetResources[i]) {
    m_pMainRenderTargetResources[i]->Release();
    m_pMainRenderTargetResources[i] = NULL;
   }
  }
#endif
 }
 inline void ImguiBase::WaitForLastSubmittedFrame() {
#if IMGUI_WIN32_DIRECTX9

#elif IMGUI_WIN32_DIRECTX12
  FrameContext* frameCtx = &m_FrameContexts[m_nFrameIndex % NUM_FRAMES_IN_FLIGHT];
  UINT64 fenceValue = frameCtx->FenceValue;
  do {
   if (fenceValue == 0)
    break;
   frameCtx->FenceValue = 0;
   if (m_pFence->GetCompletedValue() >= fenceValue)
    break;
   m_pFence->SetEventOnCompletion(fenceValue, m_hFenceEvent);
   WaitForSingleObject(m_hFenceEvent, INFINITE);
  } while (0);
#endif
 }

#if IMGUI_WIN32_DIRECTX12
 inline FrameContext* ImguiBase::WaitForNextFrameResources() {
  UINT nextFrameIndex = m_nFrameIndex + 1;
  m_nFrameIndex = nextFrameIndex;
  HANDLE waitableObjects[] = { m_hSwapChainWaitableObject, NULL };
  DWORD numWaitableObjects = 1;
  FrameContext* frameCtx = &m_FrameContexts[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
  UINT64 fenceValue = frameCtx->FenceValue;
  if (fenceValue != 0) // means no fence was signaled
  {
   frameCtx->FenceValue = 0;
   m_pFence->SetEventOnCompletion(fenceValue, m_hFenceEvent);
   waitableObjects[1] = m_hFenceEvent;
   numWaitableObjects = 2;
  }
  WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
  return frameCtx;
 }
#endif
 inline LRESULT WINAPI ImguiBase::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
   return true;
  static ImguiBase* _pThis = nullptr;
  switch (msg) {
  case WM_CREATE: {
   LPCREATESTRUCTW pCreateStructW = reinterpret_cast<LPCREATESTRUCTW>(lParam);
   if (!pCreateStructW)
    break;
   _pThis = reinterpret_cast<ImguiBase*>(pCreateStructW->lpCreateParams);
   if (!_pThis)
    break;
   auto sk = 0;
  }break;
  case WM_SIZE: {
   if (!_pThis)
    break;
#if IMGUI_WIN32_DIRECTX9
   if (_pThis->m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
   {
    _pThis->m_d3dpp.BackBufferWidth = LOWORD(lParam);
    _pThis->m_d3dpp.BackBufferHeight = HIWORD(lParam);
    _pThis->ResetDevice();
   }
#elif IMGUI_WIN32_DIRECTX10
   if (_pThis->m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
    _pThis->CleanupRenderTarget();
    _pThis->m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
    _pThis->CreateRenderTarget();
   }
#elif IMGUI_WIN32_DIRECTX11
   if (_pThis->m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
   {
    _pThis->CleanupRenderTarget();
    _pThis->m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
    _pThis->CreateRenderTarget();
   }
#elif IMGUI_WIN32_DIRECTX12
   if (!_pThis->m_pD3D12Device)
    break;
   if (wParam == SIZE_MINIMIZED)
    break;
   if (!_pThis->m_pSwapChain)
    break;
   _pThis->WaitForLastSubmittedFrame();
   _pThis->CleanupRenderTarget();
   HRESULT result = _pThis->m_pSwapChain->ResizeBuffers(
    0,
    (UINT)LOWORD(lParam),
    (UINT)HIWORD(lParam),
    DXGI_FORMAT_UNKNOWN,
    DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
   if (!SUCCEEDED(result))
    break;
   _pThis->CreateRenderTarget();
#endif
   return 0;
  }break;
  case WM_SYSCOMMAND: {
   if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
    return 0;
  }break;
   break;
  case WM_DESTROY: {
   ::PostQuitMessage(0);
   return 0;
  }break;
  default: {
  }break;
  }
  return ::DefWindowProcW(hWnd, msg, wParam, lParam);
 }
#endif


 inline unsigned int __stdcall ImguiBase::MainProcess(void* arg) {
  ImguiBase* _This = reinterpret_cast<ImguiBase*>(arg);
  if (!_This)
   return -1;
#if IMGUI_WIN32_DIRECTX9
  // Create application window
//ImGui_ImplWin32_EnableDpiAwareness();
  WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
  ::RegisterClassExW(&wc);
  m_hWnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, this);
  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd))
  {
   CleanupDeviceD3D();
   ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
   return 1;
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX9_Init(m_pd3dDevice);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done)
  {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
   {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
     done = true;
   }
   if (done)
    break;

   // Start the Dear ImGui frame
   ImGui_ImplDX9_NewFrame();
   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }

   // Rendering
   ImGui::EndFrame();
   m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
   m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   m_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
   D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
   m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
   if (m_pd3dDevice->BeginScene() >= 0)
   {
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    m_pd3dDevice->EndScene();
   }
   HRESULT result = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

   // Handle loss of D3D9 device
   if (result == D3DERR_DEVICELOST && m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    ResetDevice();
  }

  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
#elif IMGUI_WIN32_DIRECTX10
  // Create application window
   //ImGui_ImplWin32_EnableDpiAwareness();
  WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
  ::RegisterClassExW(&wc);
  m_hWnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX10 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd))
  {
   CleanupDeviceD3D();
   ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
   return 1;
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX10_Init(m_pd3dDevice);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done)
  {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
   {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
     done = true;
   }
   if (done)
    break;

   // Start the Dear ImGui frame
   ImGui_ImplDX10_NewFrame();
   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }

   // Rendering
   ImGui::Render();
   const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
   m_pd3dDevice->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
   m_pd3dDevice->ClearRenderTargetView(m_mainRenderTargetView, clear_color_with_alpha);
   ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

   m_pSwapChain->Present(1, 0); // Present with vsync
   //g_pSwapChain->Present(0, 0); // Present without vsync
  }

  ImGui_ImplDX10_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
#elif IMGUI_WIN32_DIRECTX11
  // Create application window
   //ImGui_ImplWin32_EnableDpiAwareness();
  WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
  ::RegisterClassExW(&wc);
  m_hWnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd))
  {
   CleanupDeviceD3D();
   ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
   return 1;
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done)
  {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
   {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
     done = true;
   }
   if (done)
    break;

   // Start the Dear ImGui frame
   ImGui_ImplDX11_NewFrame();
   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }

   // Rendering
   ImGui::Render();
   const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
   m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
   m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, clear_color_with_alpha);
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

   m_pSwapChain->Present(1, 0); // Present with vsync
   //g_pSwapChain->Present(0, 0); // Present without vsync
  }

  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
#elif IMGUI_WIN32_DIRECTX12
  // Create application window
  //ImGui_ImplWin32_EnableDpiAwareness();
  const DWORD dwCreateStyle = _This->m_hParent ? (WS_CHILD | WS_OVERLAPPEDWINDOW) : WS_OVERLAPPEDWINDOW;
  WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, ::GetModuleHandleW(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
  ::RegisterClassExW(&wc);
  _This->m_hWnd = \
   ::CreateWindowW(
    wc.lpszClassName,
    L"Dear ImGui DirectX12 Example",
    dwCreateStyle,
    100, 100, 1280, 800, _This->m_hParent, NULL, wc.hInstance, _This);

  // Initialize Direct3D
  if (!_This->CreateDeviceD3D(_This->m_hWnd))
  {
   _This->CleanupDeviceD3D();
   ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
   return 1;
  }

  // Show the window
  ::ShowWindow(_This->m_hWnd, SW_SHOWDEFAULT);
  ::UpdateWindow(_This->m_hWnd);

  ::SetEvent(_This->m_hWaitForMainWndCreate);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(_This->m_hWnd);
  ImGui_ImplDX12_Init(_This->m_pD3D12Device, NUM_FRAMES_IN_FLIGHT,
   DXGI_FORMAT_R8G8B8A8_UNORM, _This->m_pD3DSrvDescHeap,
   _This->m_pD3DSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
   _This->m_pD3DSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Main loop
  bool done = false;
  while (!done)
  {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE))
   {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
     done = true;
   }
   if (done)
    break;

   // Start the Dear ImGui frame
   ImGui_ImplDX12_NewFrame();
   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }
   // Rendering
   ImGui::Render();
   FrameContext* frameCtx = _This->WaitForNextFrameResources();
   UINT backBufferIdx = _This->m_pSwapChain->GetCurrentBackBufferIndex();
   frameCtx->CommandAllocator->Reset();
   D3D12_RESOURCE_BARRIER barrier = {};
   barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
   barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
   barrier.Transition.pResource = _This->m_pMainRenderTargetResources[backBufferIdx];
   barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
   barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
   barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
   _This->m_pD3DCommandList->Reset(frameCtx->CommandAllocator, NULL);
   _This->m_pD3DCommandList->ResourceBarrier(1, &barrier);
   // Render Dear ImGui graphics
   const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
   _This->m_pD3DCommandList->ClearRenderTargetView(_This->m_pMainRenderTargetDescriptors[backBufferIdx], clear_color_with_alpha, 0, NULL);
   _This->m_pD3DCommandList->OMSetRenderTargets(1, &_This->m_pMainRenderTargetDescriptors[backBufferIdx], FALSE, NULL);
   _This->m_pD3DCommandList->SetDescriptorHeaps(1, &_This->m_pD3DSrvDescHeap);
   ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _This->m_pD3DCommandList);
   barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
   barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
   _This->m_pD3DCommandList->ResourceBarrier(1, &barrier);
   _This->m_pD3DCommandList->Close();
   _This->m_pD3DCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&_This->m_pD3DCommandList);
   _This->m_pSwapChain->Present(1, 0); // Present with vsync
   //g_pSwapChain->Present(0, 0); // Present without vsync
   UINT64 fenceValue = _This->m_nFenceLastSignaledValue + 1;
   _This->m_pD3DCommandQueue->Signal(_This->m_pFence, fenceValue);
   _This->m_nFenceLastSignaledValue = fenceValue;
   frameCtx->FenceValue = fenceValue;
  }
  _This->WaitForLastSubmittedFrame();
  // Cleanup
  ImGui_ImplDX12_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  _This->CleanupDeviceD3D();
  ::DestroyWindow(_This->m_hWnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
#elif IMGUI_GLFW_OPENGL2
  glfwSetErrorCallback([](int, const char*) {});
  if (!glfwInit())
   return 1;

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL2 example", NULL, NULL);
  if (window == NULL)
   return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL2_Init();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
   // Poll and handle events (inputs, window resize, etc.)
   // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
   // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
   // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
   // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
   glfwPollEvents();

   // Start the Dear ImGui frame
   ImGui_ImplOpenGL2_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }

   // Rendering
   ImGui::Render();
   int display_w, display_h;
   glfwGetFramebufferSize(window, &display_w, &display_h);
   glViewport(0, 0, display_w, display_h);
   glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   glClear(GL_COLOR_BUFFER_BIT);

   // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
   // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
   //GLint last_program;
   //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
   //glUseProgram(0);
   ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
   //glUseProgram(last_program);

   glfwMakeContextCurrent(window);
   glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
#elif IMGUI_GLFW_OPENGL3
  glfwSetErrorCallback([](int, const char*) {});
  if (!glfwInit())
   return 1;

  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL)
   return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
  io.IniFilename = NULL;
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
  while (!glfwWindowShouldClose(window))
#endif
  {
   // Poll and handle events (inputs, window resize, etc.)
   // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
   // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
   // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
   // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
   glfwPollEvents();

   // Start the Dear ImGui frame
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
   {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
     counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
     show_another_window = false;
    ImGui::End();
   }

   // Rendering
   ImGui::Render();
   int display_w, display_h;
   glfwGetFramebufferSize(window, &display_w, &display_h);
   glViewport(0, 0, display_w, display_h);
   glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   glClear(GL_COLOR_BUFFER_BIT);
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

   glfwSwapBuffers(window);
  }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
#elif IMGUI_GLUT_OPENGL2
  // Create GLUT window
  glutInit(&__argc, __argv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(m_WindowSize.cx, m_WindowSize.cy);
  if (1 != glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example"))
   break;
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  m_pImGuiIO = &GetIO();
  if (!m_pImGuiIO)
   break;
  m_pImGuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  // Setup Platform/Renderer backends
  // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
  ImGui_ImplGLUT_Init();
  ImGui_ImplOpenGL2_Init();
  ImGui_ImplGLUT_InstallFuncs();

  glutDisplayFunc(
   []() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    if (show_demo_window)
     ImGui::ShowDemoWindow(&show_demo_window);
    {
     static float f = 0.0f;
     static int counter = 0;
     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
     ImGui::Checkbox("Another Window", &show_another_window);
     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      counter++;
     ImGui::SameLine();
     ImGui::Text("counter = %d", counter);
     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GetIO().Framerate, GetIO().Framerate);
     ImGui::End();
    }
    if (show_another_window) {
     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
     ImGui::Text("Hello from another window!");
     if (ImGui::Button("Close Me"))
      show_another_window = false;
     ImGui::End();
    }
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
    glutPostRedisplay();
   });

  glutMainLoop();

  m_RenderThread = std::thread(
   [&]() {
    do {
     if (!m_IsOpen.load())
      break;
    } while (0);
 });
#endif



  return 0;
}
}///namespace ImGui



#pragma comment(lib,"imgui.lib")
/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 01 Apr 2023 23:52:24 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0A8B1668_73C2_46DD_BD4A_5A35A7A176C0__


