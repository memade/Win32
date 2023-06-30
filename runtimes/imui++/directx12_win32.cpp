#include "stdafx.h"

#if IMGUI_WIN32_DIRECTX12
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

namespace local {
 Directx12Drive::Directx12Drive(const IDearImGui* host) : IDrive(host) {

 }

 Directx12Drive::~Directx12Drive() {

 }

 bool Directx12Drive::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void Directx12Drive::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }
 void Directx12Drive::Release() const {
  delete this;
 }

 bool Directx12Drive::CreateDeviceD3D(HWND hWnd) {
  bool result = false;
  do {
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
   result = true;
  } while (0);
  return result;
 }
 void Directx12Drive::CleanupDeviceD3D() {
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
 }
 void Directx12Drive::CreateRenderTarget() {
  for (UINT i = 0; i < NUM_BACK_BUFFERS; i++) {
   ID3D12Resource* pBackBuffer = NULL;
   m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
   m_pD3D12Device->CreateRenderTargetView(pBackBuffer, NULL, m_pMainRenderTargetDescriptors[i]);
   m_pMainRenderTargetResources[i] = pBackBuffer;
  }
 }
 void Directx12Drive::CleanupRenderTarget() {
  WaitForLastSubmittedFrame();
  for (UINT i = 0; i < NUM_BACK_BUFFERS; i++) {
   if (m_pMainRenderTargetResources[i]) {
    m_pMainRenderTargetResources[i]->Release();
    m_pMainRenderTargetResources[i] = NULL;
   }
  }
 }
 void Directx12Drive::WaitForLastSubmittedFrame() {
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
 }
 void Directx12Drive::ResetDevice() {

 }
 FrameContext* Directx12Drive::WaitForNextFrameResources() {
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


 LRESULT WINAPI Directx12Drive::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
   return true;
  static Directx12Drive* _pThis = nullptr;
  switch (msg) {
  case WM_CREATE: {
   LPCREATESTRUCTW pCreateStructW = reinterpret_cast<LPCREATESTRUCTW>(lParam);
   if (!pCreateStructW)
    break;
   _pThis = reinterpret_cast<Directx12Drive*>(pCreateStructW->lpCreateParams);
   if (!_pThis)
    break;
   auto sk = 0;
  }break;
  case WM_SIZE: {
   if (!_pThis)
    break;

   do {
    HWND parentWnd = GetParent(hWnd);
    if (!parentWnd)
     break;
    RECT parentRect;
    GetClientRect(parentWnd, &parentRect);
    int parentWidth = parentRect.right - parentRect.left;
    int parentHeight = parentRect.bottom - parentRect.top;
    MoveWindow(hWnd, 0, 0, parentWidth, parentHeight, TRUE);
   } while (0);

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
   return 0;
  }break;
  case WM_SYSCOMMAND: {
   if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
    return 0;
  }break;
   break;
  case WM_DESTROY: {
   /*_pThis->OnDestroy();*/
   ::PostQuitMessage(0);
   return 0;
  }break;
  default: {
  }break;
  }
  return ::DefWindowProcW(hWnd, msg, wParam, lParam);
 }

 void Directx12Drive::Process() {
  m_WndClassEx.cbSize = sizeof(m_WndClassEx);
  m_WndClassEx.style = CS_CLASSDC;
  m_WndClassEx.lpfnWndProc = WindowProc;
  m_WndClassEx.cbClsExtra = 0L;
  m_WndClassEx.cbWndExtra = 0L;
  m_WndClassEx.hInstance = __gpHinstance;
  m_WndClassEx.hIcon = NULL;
  m_WndClassEx.hCursor = NULL;
  m_WndClassEx.hbrBackground = NULL;
  m_WndClassEx.lpszMenuName = NULL;
  m_WndClassEx.lpszClassName = L"ImGui®";
  m_WndClassEx.hIconSm = NULL;
  // Create application window
  ImGui_ImplWin32_EnableDpiAwareness();
  void(::RegisterClassExW(&m_WndClassEx));
  //CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y, \
     // nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
  DWORD dwExStyle = 0;
  LPCWSTR lpClassName = m_WndClassEx.lpszClassName;
  LPCWSTR lpWindowName = L"Dear ImGui DirectX12 Example";
  DWORD dwStyle = WS_OVERLAPPEDWINDOW;
  int X = 0;
  int Y = 0;
  int nWidth = 1280;
  int nHeight = 800;
  HWND hWndParent = nullptr/*_This->ConfigGet()->Parent()*/;
  HMENU hMenu = NULL;
  HINSTANCE hInstance = m_WndClassEx.hInstance;
  LPVOID lpParam = this;
#if 0
  __this->OnCreateWindow(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#endif
  m_hWnd = ::CreateWindowExW(
   dwExStyle,
   lpClassName,
   lpWindowName,
   dwStyle,
   X,
   Y,
   nWidth,
   nHeight,
   hWndParent,
   hMenu,
   hInstance,
   this);
  if (!m_hWnd)
   return;
  if (!CreateDeviceD3D(m_hWnd)) {
   CleanupDeviceD3D();
   ::UnregisterClassW(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
   return;
  }
  ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
  ::UpdateWindow(m_hWnd);
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(m_hWnd);
  ImGui_ImplDX12_Init(m_pD3D12Device, NUM_FRAMES_IN_FLIGHT,
   DXGI_FORMAT_R8G8B8A8_UNORM, m_pD3DSrvDescHeap,
   m_pD3DSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
   m_pD3DSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

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
  while (!done) {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
    if (msg.message == WM_QUIT)
     done = true;
   }
   if (done)
    break;

   // Start the Dear ImGui frame
   ImGui_ImplDX12_NewFrame();
   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

#if 0
   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (true)
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

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
#endif
   Renderer();


   // Rendering
   ImGui::Render();
   FrameContext* frameCtx = WaitForNextFrameResources();
   UINT backBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
   frameCtx->CommandAllocator->Reset();
   D3D12_RESOURCE_BARRIER barrier = {};
   barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
   barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
   barrier.Transition.pResource = m_pMainRenderTargetResources[backBufferIdx];
   barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
   barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
   barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
   m_pD3DCommandList->Reset(frameCtx->CommandAllocator, NULL);
   m_pD3DCommandList->ResourceBarrier(1, &barrier);
   // Render Dear ImGui graphics
   const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
   m_pD3DCommandList->ClearRenderTargetView(m_pMainRenderTargetDescriptors[backBufferIdx], clear_color_with_alpha, 0, NULL);
   m_pD3DCommandList->OMSetRenderTargets(1, &m_pMainRenderTargetDescriptors[backBufferIdx], FALSE, NULL);
   m_pD3DCommandList->SetDescriptorHeaps(1, &m_pD3DSrvDescHeap);
   ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_pD3DCommandList);
   barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
   barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
   m_pD3DCommandList->ResourceBarrier(1, &barrier);
   m_pD3DCommandList->Close();
   m_pD3DCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&m_pD3DCommandList);
   m_pSwapChain->Present(1, 0); // Present with vsync
   //g_pSwapChain->Present(0, 0); // Present without vsync
   UINT64 fenceValue = m_nFenceLastSignaledValue + 1;
   m_pD3DCommandQueue->Signal(m_pFence, fenceValue);
   m_nFenceLastSignaledValue = fenceValue;
   frameCtx->FenceValue = fenceValue;
  }

  WaitForLastSubmittedFrame();
  ImGui_ImplDX12_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  CleanupDeviceD3D();
  ::DestroyWindow(m_hWnd);
  ::UnregisterClassW(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
 }
}///namespace local
#endif
