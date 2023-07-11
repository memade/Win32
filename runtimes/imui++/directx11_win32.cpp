﻿#include "stdafx.h"

#if IMGUI_WIN32_DIRECTX11
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
namespace local {


 Directx11Drive::Directx11Drive(const IDearImGui* host, const Control* ui_ctrl) : IDrive(host, ui_ctrl) {
  m_WndClassEx.lpfnWndProc = Directx11Drive::WindowProc;
  m_WndCreateStructW.lpCreateParams = this;
 }

 Directx11Drive::~Directx11Drive() {

 }
 void Directx11Drive::Release() const {
  delete this;
 }
 bool Directx11Drive::Create() {
  bool result = false;
  do {
   void(::RegisterClassExW(&m_WndClassEx));
   m_hWnd = ::CreateWindowW(
    m_WndCreateStructW.lpszClass,
    m_WndCreateStructW.lpszName,
    m_WndCreateStructW.style,
    m_WndCreateStructW.cx,
    m_WndCreateStructW.cy,
    m_WndCreateStructW.x,
    m_WndCreateStructW.y,
    m_WndCreateStructW.hwndParent,
    m_WndCreateStructW.hMenu,
    m_WndCreateStructW.hInstance,
    m_WndCreateStructW.lpCreateParams);
   if (!m_hWnd)
    break;
   ::ShowWindow(m_hWnd, SW_HIDE);
   ::UpdateWindow(m_hWnd);
   result = true;
  } while (0);
  CreateNotify();
  return result;
 }
 void Directx11Drive::Destroy() {
  do {
   // Cleanup
   ImGui_ImplDX11_Shutdown();
   ImGui_ImplWin32_Shutdown();
   ImGui::DestroyContext();

   CleanupDeviceD3D();
   ::DestroyWindow(m_hWnd);
   ::UnregisterClassW(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
  } while (0);
 }
 bool Directx11Drive::CreateDeviceD3D(HWND hWnd) {
  bool result = false;
  do {
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
   result = true;
  } while (0);
  return result;
 }
 void Directx11Drive::CleanupDeviceD3D() {
  CleanupRenderTarget();
  if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
  if (m_pd3dDeviceContext) { m_pd3dDeviceContext->Release(); m_pd3dDeviceContext = NULL; }
  if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
 }
 void Directx11Drive::CreateRenderTarget() {
  ID3D11Texture2D* pBackBuffer = nullptr;
  do {
   m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
   if (!pBackBuffer)
    break;
   m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
   pBackBuffer->Release();
  } while (0);
 }
 void Directx11Drive::CleanupRenderTarget() {
  if (m_mainRenderTargetView) {
   m_mainRenderTargetView->Release();
   m_mainRenderTargetView = NULL;
  }
 }
 void Directx11Drive::WaitForLastSubmittedFrame() {

 }
 void Directx11Drive::ResetDevice() {

 }




 void Directx11Drive::Process() {

  // Create application window
   //ImGui_ImplWin32_EnableDpiAwareness();
  
  if (!Create())
   return;
  // Initialize Direct3D
  if (!CreateDeviceD3D(m_hWnd))
  {
   CleanupDeviceD3D();
   ::UnregisterClassW(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
   return;
  }

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
  ImGui_ImplWin32_Init(m_hWnd);
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

#if 1
   OnRender();
#else
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
#endif

   ImGui::EndFrame();
   // Rendering
   ImGui::Render();
   const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
   m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
   m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, clear_color_with_alpha);
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

   m_pSwapChain->Present(1, 0); // Present with vsync
   //g_pSwapChain->Present(0, 0); // Present without vsync
  }
  Destroy();
 }

 LRESULT WINAPI Directx11Drive::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
   return true;
  static Directx11Drive* _pThis = nullptr;
  switch (msg) {
  case WM_CREATE: {
   LPCREATESTRUCTW pCreateStructW = reinterpret_cast<LPCREATESTRUCTW>(lParam);
   if (!pCreateStructW)
    break;
   _pThis = reinterpret_cast<Directx11Drive*>(pCreateStructW->lpCreateParams);
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

   if (_pThis->m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
   {
    _pThis->CleanupRenderTarget();
    _pThis->m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
    _pThis->SetSize(Vec2(LOWORD(lParam), HIWORD(lParam)));
    _pThis->CreateRenderTarget();
   }
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


}///namespace local
#endif
