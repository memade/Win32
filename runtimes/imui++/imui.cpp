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
 }
 void DearImGui::UnInit() {
  if (m_pConfig) {
   m_pConfig->Release();
   m_pConfig = nullptr;
  }
 }
 void DearImGui::SkinDestroy() {
  skin::ISkin::DestoryInterface(m_pSkin);
 }
 bool DearImGui::SkinCreate(const char* skin_file_pathanme) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!skin_file_pathanme)
    break;
   if (!shared::Win::AccessA(skin_file_pathanme))
    break;
   m_pSkin = skin::ISkin::CreateInterface(shared::Win::GetModulePathA(__gpHinstance) + R"(/skin++.dll)");
   if (!m_pSkin)
    break;
   m_pSkin->SetUIModule(dynamic_cast<ISkinUI*>(this));
   if (!m_pSkin->SkinConfigure(skin_file_pathanme))
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

   skin::ISkin::DestoryInterface(m_pSkin);
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

 IControlUI* DearImGui::CreateControl(const ControlType& type) {
  IControlUI* result = nullptr;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  switch (type) {
  case ControlType::Window: {
   result = dynamic_cast<IControlUI*>(new Window(this));
  }break;
  case ControlType::Frame: {
   result = dynamic_cast<IControlUI*>(new Frame());
  }break;
  case ControlType::VerticalLayout: {
   result = dynamic_cast<IControlUI*>(new VerticalLayout());
  }break;
  case ControlType::HorizontalLayout: {
   result = dynamic_cast<IControlUI*>(new HorizontalLayout());
  }break;
  case ControlType::Button: {
   result = dynamic_cast<IControlUI*>(new Button());
  }break;
  default:
   break;
  }
  return result;
 }





 extern HINSTANCE __gpHinstance = nullptr;
}///namespace local