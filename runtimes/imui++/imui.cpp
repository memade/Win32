#include "stdafx.h"

namespace local {
 // Our state
 static bool show_demo_window = true;
 static bool show_another_window = false;
 static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 DearImGui::DearImGui() {
  Init();
 }
 DearImGui:: ~DearImGui() {
  UnInit();
 }
 void DearImGui::Release() const {
  delete this;
 }
 void DearImGui::Init() {
  m_pConfig = dynamic_cast<shared::IUIConfig*>(new Config());
#if IMGUI_GLUT_OPENGL2
  m_pDrive = dynamic_cast<IDrive*>(new OpenGL2GlutDrive(this));
#elif IMGUI_GLFW_OPENGL2
  m_pDrive = dynamic_cast<IDrive*>(new OpenGL2GlfwDrive(this));
#elif IMGUI_GLFW_OPENGL3
  m_pDrive = dynamic_cast<IDrive*>(new OpenGL3GlfwDrive(this));
#elif IMGUI_WIN32_DIRECTX9
  m_pDrive = dynamic_cast<IDrive*>(new Directx9Drive(this));
#elif IMGUI_WIN32_DIRECTX10
  m_pDrive = dynamic_cast<IDrive*>(new Directx10Drive(this));
#elif IMGUI_WIN32_DIRECTX11
  m_pDrive = dynamic_cast<IDrive*>(new Directx11Drive(this));
#elif IMGUI_WIN32_DIRECTX12
  m_pDrive = dynamic_cast<IDrive*>(new Directx12Drive(this));
#endif
 }
 void DearImGui::UnInit() {
  if (m_pDrive) {
   m_pDrive->Release();
   m_pDrive = nullptr;
  }
  if (m_pConfig) {
   m_pConfig->Release();
   m_pConfig = nullptr;
  }
 }
 void DearImGui::Render() {

  auto sk = 0;
 }
 bool DearImGui::CreateMainWindow() {
  return false;
 }

 bool DearImGui::Skin(const char* skin_directory) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!skin_directory)
    break;
   if (!shared::Win::AccessA(skin_directory))
    break;
   m_pSkin = skin::ISkin::CreateInterface(shared::Win::GetModulePathA(__gpHinstance) + R"(/skin++.dll)");
   if (!m_pSkin)
    break;
   m_pSkin->RegisterNodeCreateCb(
    [&](skin::INodeRender* pINode) {
     switch (pINode->GetType()) {
     case skin::ControlType::Window:
      pINode->Route(reinterpret_cast<void*>(dynamic_cast<ICtrl*>(new Window(pINode))));
      break;
     case skin::ControlType::Font:
      pINode->Route(reinterpret_cast<void*>(dynamic_cast<ICtrl*>(new Font(pINode))));
      break;
     case skin::ControlType::Frame:
      pINode->Route(reinterpret_cast<void*>(dynamic_cast<ICtrl*>(new Frame(pINode))));
      break;
     case skin::ControlType::Control:
      break;
     case skin::ControlType::Container:
      break;
     case skin::ControlType::Button:
      break;
     case skin::ControlType::TabLayout:
      break;
     case skin::ControlType::VerticalLayout:
      pINode->Route(reinterpret_cast<void*>(dynamic_cast<ICtrl*>(new VerticalLayout(pINode))));
      break;
     case skin::ControlType::HorizontalLayout:
      pINode->Route(reinterpret_cast<void*>(dynamic_cast<ICtrl*>(new HorizontalLayout(pINode))));
      break;
     case skin::ControlType::Label:
      break;
     default:
      break;
     }
    });
   m_pSkin->RegisterNodeDestroyCb(
    [&](skin::INodeRender* pINode) {
     auto pCtrl = reinterpret_cast<ICtrl*>(pINode->Route());
     if (pCtrl)
      pCtrl->Release();
    });
   m_pSkin->From(dynamic_cast<ISkinUI*>(this));
   if (!m_pSkin->From(skin_directory))
    break;
   result = true;
  } while (0);
  return result;
 }
 shared::IUIConfig* DearImGui::UIConfigGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pConfig;
 }
 class ISkin* DearImGui::SkinGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pSkin;
 }
 bool DearImGui::Start() {
  do {
   if (m_IsOpen.load())
    break;
   if (!m_pSkin->Perform())
    break;

   if (!m_pDrive)
    break;
   if (!m_pDrive->Start())
    break;

   m_IsOpen.store(true);

#if 0
   HANDLE hMainCreate = ::CreateEventW(NULL, TRUE, FALSE, NULL);
   auto tieRoute = std::make_tuple(this, &hMainCreate);
   m_hMain = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, MainProcess, &tieRoute, 0, NULL));
   if (!m_hMain)
    break;
   if (hMainCreate) {
    ::WaitForSingleObject(hMainCreate, INFINITE);
    SK_CLOSE_HANDLE(hMainCreate);
   }
#endif
  } while (0);
  return m_IsOpen.load();
 }
 void DearImGui::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);

   if (m_pDrive)
    m_pDrive->Stop();

   skin::ISkin::DestoryInterface(m_pSkin);
  } while (0);
 }
 void DearImGui::Center() const {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  do {
   if (!m_pDrive)
    break;
   if (!SkinGet() || !SkinGet()->MainNode())
    break;
   lock.lock();
   HWND hWnd = m_pDrive->Handle();
   lock.unlock();
   if (!hWnd)
    break;

   RECT rtWindow;
   ::GetWindowRect(hWnd, &rtWindow);
   // 获取屏幕尺寸
   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);
   // 计算窗口位置
   int windowWidth = rtWindow.right - rtWindow.left;
   int windowHeight = rtWindow.bottom - rtWindow.top;
   int windowX = (screenWidth - windowWidth) / 2;
   int windowY = (screenHeight - windowHeight) / 2;
   // 移动窗口到屏幕中心
   SetWindowPos(hWnd, NULL, windowX, windowY, windowWidth, windowHeight,
    SkinGet()->MainNode()->Visible() ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
  } while (0);
 }
 void DearImGui::NotifyUICreateSuccess() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_hEventCreateUI)
   ::SetEvent(m_hEventCreateUI);
 }
 unsigned int __stdcall DearImGui::MainProcess(void* arg) {
  auto pTieroute = reinterpret_cast<std::tuple<DearImGui*, HANDLE*>*>(arg);
  assert(pTieroute);
  DearImGui* _This = std::get<0>(*pTieroute);
  HANDLE hCreate = *std::get<1>(*pTieroute);
  assert(_This && hCreate);

  if (hCreate)
   ::SetEvent(hCreate);
  return 0;
 }


 extern HINSTANCE __gpHinstance = nullptr;
}///namespace local