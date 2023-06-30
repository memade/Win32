#include "stdafx.h"

#if IMGUI_WIN32_DIRECTX9
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

namespace local {

 Directx9Drive::Directx9Drive(const IDearImGui* host) : IDrive(host) {

 }

 Directx9Drive::~Directx9Drive() {

 }
 void Directx9Drive::Release() const {
  delete this;
 }
 bool Directx9Drive::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   //m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void Directx9Drive::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }
 bool Directx9Drive::CreateDeviceD3D(HWND hWnd) {
  bool result = false;
  do {
   if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
    break;
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
   result = true;
  } while (0);
  return result;
 }
 void Directx9Drive::CleanupDeviceD3D() {
  if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
  if (m_pD3D) { m_pD3D->Release(); m_pD3D = NULL; }
 }
 void Directx9Drive::CreateRenderTarget() {

 }
 void Directx9Drive::CleanupRenderTarget() {

 }
 void Directx9Drive::WaitForLastSubmittedFrame() {

 }
 void Directx9Drive::ResetDevice() {
  ImGui_ImplDX9_InvalidateDeviceObjects();
  HRESULT hr = m_pd3dDevice->Reset(&m_d3dpp);
  if (hr == D3DERR_INVALIDCALL)
   IM_ASSERT(0);
  ImGui_ImplDX9_CreateDeviceObjects();
 }

 void Directx9Drive::Process() {

  if (m_pHost && m_pHost->UIConfigGet()->EnableDpiAwareness()) {
   ImGui_ImplWin32_EnableDpiAwareness();
  }

  if (!OnCreateApplicationWindow(this, Directx9Drive::WindowProc))
   return;

  ::UpdateWindow(m_hWnd);

  // Initialize Direct3D
  if (!CreateDeviceD3D(m_hWnd)) {
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
  ImGui_ImplDX9_Init(m_pd3dDevice);

  OnLoadFonts();
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
  //auto font = io.Fonts->AddFontFromFileTTF(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\skin\fonts\msyh.ttf)", 18.0f);
  //IM_ASSERT(font != NULL);
  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  bool done = false;
  do {
   // Poll and handle messages (inputs, window resize, etc.)
   // See the WndProc() function below for our to dispatch events to the Win32 backend.
   MSG msg;
   while (::PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
    if (msg.message == WM_QUIT) {
     done = true;
     break;
    }
   }
   if (done)
    break;
   // Start the Dear ImGui frame
   ImGui_ImplDX9_NewFrame();
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
   // Rendering
   ImGui::EndFrame();
   m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
   m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   m_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
   D3DCOLOR main_bkcolor = 0;
   OnRefreshBkcolor(main_bkcolor);
   m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, main_bkcolor, 1.0f, 0);
   if (m_pd3dDevice->BeginScene() >= 0) {
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    m_pd3dDevice->EndScene();
   }
   HRESULT result = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

   // Handle loss of D3D9 device
   if (result == D3DERR_DEVICELOST && m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    ResetDevice();
   dynamic_cast<const DearImGui*>(m_pHost)->NotifyUICreateSuccess();
 } while (1);

 ImGui_ImplDX9_Shutdown();
 ImGui_ImplWin32_Shutdown();
 ImGui::DestroyContext();

 CleanupDeviceD3D();
 ::DestroyWindow(m_hWnd);
 ::UnregisterClassW(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
}

















 LRESULT WINAPI Directx9Drive::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
   return true;
  static Directx9Drive* spThis = nullptr;
  switch (msg) {
  case WM_CREATE: {
   LPCREATESTRUCTW pCreateStructW = reinterpret_cast<LPCREATESTRUCTW>(lParam);
   if (!pCreateStructW)
    break;
   spThis = reinterpret_cast<Directx9Drive*>(pCreateStructW->lpCreateParams);
   if (!spThis)
    break;
  }break;
  case WM_SIZE: {
   if (!spThis)
    break;

   if (spThis->m_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
    spThis->m_d3dpp.BackBufferWidth = LOWORD(lParam);
    spThis->m_d3dpp.BackBufferHeight = HIWORD(lParam);
    spThis->ResetDevice();
   }
   return 0;
  }break;
  case WM_GETMINMAXINFO: {
   if (!spThis || !spThis->Host() || !spThis->Host()->SkinGet())
    break;
   auto pStyle = spThis->Host()->SkinGet()->MainNode();
   if (!pStyle)
    break;
   // 获取最大化信息结构体指针
   LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
   // 设置最大化宽度和高度
   if (pStyle->Maxinfo()->cx > 0)
    lpMMI->ptMaxSize.x = pStyle->Maxinfo()->cx;
   if (pStyle->Maxinfo()->cy > 0)
    lpMMI->ptMaxSize.y = pStyle->Maxinfo()->cy;
   if (pStyle->Mininfo()->cx > 0)
    lpMMI->ptMinTrackSize.x = pStyle->Mininfo()->cx;
   if (pStyle->Mininfo()->cy > 0)
    lpMMI->ptMinTrackSize.y = pStyle->Mininfo()->cy;
   // 设置最大化位置
   lpMMI->ptMaxPosition.x = 0;
   lpMMI->ptMaxPosition.y = 0;
   return 0;
  }break;
  case WM_SYSCOMMAND: {
   if ((wParam & 0xFFF0) == SC_KEYMENU) // Disable ALT application menu
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

