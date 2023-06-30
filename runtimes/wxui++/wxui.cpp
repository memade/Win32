#include "stdafx.h"

namespace local {
 Wxui::Wxui() {
  Init();
 }
 Wxui::~Wxui() {
  UnInit();
 }
 void Wxui::Release() const {
  delete this;
 }
 void Wxui::Init() {
  m_pConfig = dynamic_cast<shared::IUIConfig*>(new Config());
 }
 void Wxui::UnInit() {
  m_pConfig->Release();
 }
 shared::IUIConfig* Wxui::UIConfigGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pConfig;
 }
 ITreeCtrl* Wxui::CreateTreeCtrl(IControl* info) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return nullptr;
 }
 ITreeList* Wxui::CreateTreeList(IControl* info) {
  ITreeList* result = nullptr;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!info)
    break;
   wxWindow* parent = nullptr;
   if (info->Parent())
    parent = reinterpret_cast<wxWindow*>(info->Parent()->Handle());
   result = dynamic_cast<ITreeList*>(new TreeList(parent));

  } while (0);
  return result;
 }
 IFrame* Wxui::CreateFrame(IControl* info) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!info)
    break;
   switch (info->FrameTypeGet()) {
   case FrameType::FRAME_MDI: {
    m_pFrame = new MDIFrame(this);
   }break;
   case FrameType::FRAME_SDI: {
    m_pFrame = new Frame(this);
   }break;
   default:
    break;
   }
   if (!m_pFrame)
    break;
   info->Handle(reinterpret_cast<void*>(m_pFrame->Handle()));
  } while (0);
  return m_pFrame;
 }
#if 0
 bool Wxui::CreateFrame(IFrame* config) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!config)
    break;
   if (config->Type() == FrameType::SDI) {

   }
   else if (config->Type() == FrameType::MDI) {

   }
   else
    break;
   result = true;
  } while (0);
  return result;
 }
#endif
 ISkin* Wxui::SkinGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pSkin;
 }
 bool Wxui::Skin(const char* skin_directory) {
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
   m_pSkin->From(dynamic_cast<ISkinUI*>(this));
   if (!m_pSkin->From(skin_directory))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Wxui::Start() {
  do {
   if (m_IsOpen.load())
    break;
   HANDLE hMainCreate = ::CreateEventW(NULL, TRUE, FALSE, NULL);
   auto tieRoute = std::make_tuple(this, &hMainCreate);
   m_hMain = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, MainThread, &tieRoute, 0, NULL));
   if (!m_hMain)
    break;
   if (hMainCreate)
    ::WaitForSingleObject(hMainCreate, INFINITE);
   SK_CLOSE_HANDLE(hMainCreate);


   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void Wxui::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);

   if (wxTheApp)
    wxTheApp->Exit();

   ::WaitForSingleObject(m_hMain, INFINITE);
   SK_CLOSE_HANDLE(m_hMain);

   skin::ISkin::DestoryInterface(m_pSkin);
  } while (0);
 }

 unsigned int __stdcall Wxui::MainThread(void* arg) {
  auto pArgTie = reinterpret_cast<std::tuple<Wxui*, HANDLE*>*>(arg);
  assert(pArgTie && "MainThread() Input arg is empty.");

  Wxui* pWxui = std::get<0>(*pArgTie);
  HANDLE hCreateEvent = *std::get<1>(*pArgTie);
  assert(pWxui && hCreateEvent);

  do {
   auto app = new App(pWxui);
   wxApp::SetInstance(app);

   if (false == wxEntryStart(__gpHinstance))
    break;
   if (false == wxTheApp->OnInit())
    break;

   pWxui->SkinGet()->Perform();

   void(::SetEvent(hCreateEvent));
   wxTheApp->OnRun();
   wxTheApp->OnExit();
   wxEntryCleanup();
  } while (0);

  if (hCreateEvent)
   void(::SetEvent(hCreateEvent));
  return 0;
 }

#if 0
 Wxui::Wxui() {
  Init();
 }

 Wxui::~Wxui() {
  UnInit();
 }

 void Wxui::Release() const {
  delete this;
 }

 void Wxui::Init() {
  m_pConfig = new Config();
 }

 void Wxui::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }
 shared::IUIConfig* Wxui::UIConfigGet() const {
  return dynamic_cast<shared::IUIConfig*>(m_pConfig);
 }
 IConfig* Wxui::ConfigGet() const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  return dynamic_cast<IConfig*>(m_pConfig);
 }
 IFrame* Wxui::FrameGet() const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  return m_pFrame;
 }
 void Wxui::MDIFrameWindowCascade() {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   m_pFrame->MDIWindowCascade();
 }
 void Wxui::MainWindowPos(const bool& auto_adjust /*= true*/, const RECT& rtPosition /*= { 0,0,0,0 }*/) {
  auto frame = dynamic_cast<wxFrame*>(m_pFrame);
  if (!frame)
   return;
  do {
   if (auto_adjust || !m_pFrame)
    break;
   frame->SetPosition(wxPoint(rtPosition.left, rtPosition.top));
   frame->SetSize(wxSize(rtPosition.right - rtPosition.left, rtPosition.bottom - rtPosition.top));
  } while (0);

  do {//!@ 自动调整到当前显示器比例
   if (!auto_adjust || !m_pFrame)
    break;
   HDC hdcScreen = ::GetDC(NULL); // 获取屏幕设备上下文
   if (!hdcScreen)
    break;
   int screenWidth = ::GetDeviceCaps(hdcScreen, HORZRES); // 获取屏幕宽度
   int screenHeight = ::GetDeviceCaps(hdcScreen, VERTRES); // 获取屏幕高度
   frame->SetPosition(wxPoint(0 + 50, 0 + 20));
   frame->SetSize(wxSize(screenWidth - 100, screenHeight - 80));
   ::ReleaseDC(NULL, hdcScreen); // 释放设备上下文
  } while (0);
 }
 void Wxui::MDIFrameWindowTileHorz() {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   m_pFrame->MDIWindowTileHorz();
 }
 bool Wxui::MDIAppendChild(const HWND& h) {
  bool result = false;
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   result = m_pFrame->MDIAppendChild(h);
  return result;
 }
 void Wxui::MDIFrameWindowTileVert() {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   m_pFrame->MDIWindowTileVert();
 }
 HWND Wxui::CreateFrameChildHost(const bool& is_show) {
  HWND result = nullptr;
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  do {
   if (!m_pFrame)
    break;
   auto child = m_pFrame->CreateChildFrameHost(is_show);
   if (!child)
    break;
   result = child->Hwnd();
  } while (0);
  return result;
 }
 HWND Wxui::CreateFrameChild(const bool& is_show) {
  HWND result = nullptr;
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  do {
   if (!m_pFrame)
    break;
   auto child = m_pFrame->CreateChildFrame();
   if (!child)
    break;
   child->Show(is_show);
   result = child->Hwnd();
  } while (0);
  return result;
 }
 void Wxui::OnAppCreate(IApp* app) const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_OnAppCreateCb)
   m_OnAppCreateCb(app);
 }
 void Wxui::OnDestroy() const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_OnDestroyCb)
   m_OnDestroyCb();
 }
 void Wxui::RegisterOnDestroyCb(const tfOnDestroyCb& cb) {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  m_OnDestroyCb = cb;
 }
 void Wxui::RegisterOnAppCreateCb(const tfOnAppCreateCb& cb) {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  m_OnAppCreateCb = cb;
 }
 void Wxui::Show(const bool& show) const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   m_pFrame->Show(show);
 }
 bool Wxui::Start() {
  do {
   if (m_IsOpen.load())
    break;
   HANDLE hMainCreate = ::CreateEventW(NULL, TRUE, FALSE, NULL);
   auto tieRoute = std::make_tuple(this, &hMainCreate);
   m_hMain = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, MainThread, &tieRoute, 0, NULL));
   if (!m_hMain)
    break;
   if (hMainCreate)
    ::WaitForSingleObject(hMainCreate, INFINITE);
   SK_CLOSE_HANDLE(hMainCreate);

   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }

 void Wxui::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);

   if (wxTheApp)
    wxTheApp->Exit();

   ::WaitForSingleObject(m_hMain, INFINITE);
   SK_CLOSE_HANDLE(m_hMain);
  } while (0);
 }
 HWND Wxui::MainWnd() const {
  HWND result = nullptr;
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  if (m_pFrame)
   result = m_pFrame->Hwnd();
  return result;
 }
 HANDLE Wxui::MainProcess() const {
  /*std::lock_guard<std::mutex> lock{*m_Mutex};*/
  return m_hMain;
 }
 IFrame* Wxui::CreateFrame(const FrameType& type) {
  switch (type) {
  case FrameType::MDI: {
   m_pFrame = new Frame(this);
  }break;
  case FrameType::SDI: {
   m_pFrame = new MDIFrame(this);
  }break;
  default:
   break;
  }
  return dynamic_cast<IFrame*>(m_pFrame);
 }
 unsigned int __stdcall Wxui::MainThread(void* arg) {
  auto pArgTie = reinterpret_cast<std::tuple<Wxui*, HANDLE*>*>(arg);
  assert(pArgTie && "MainThread() Input arg is empty.");

  Wxui* pWxui = std::get<0>(*pArgTie);
  HANDLE hCreateEvent = *std::get<1>(*pArgTie);
  assert(pWxui && hCreateEvent);

  do {
   auto app = new App(pWxui);
   pWxui->OnAppCreate(dynamic_cast<IApp*>(app));
   wxApp::SetInstance(app);

   if (false == wxEntryStart(__gpHinstance))
    break;
   if (false == wxTheApp->OnInit())
    break;

#if !ENABLE_SKIN_SUPPORT
   switch (pWxui->ConfigGet()->FrameTypeGet()) {
   case FrameType::SDI: {
    pWxui->m_pFrame = dynamic_cast<IFrame*>(new Frame(pWxui));
   }break;
   case FrameType::MDI: {
    pWxui->m_pFrame = dynamic_cast<IFrame*>(new MDIFrame(pWxui));
   }break;
   case FrameType::WEBVIEW: {
    pWxui->m_pFrame = dynamic_cast<IFrame*>(new FrameWeb(pWxui));
   }break;
   default:
    break;
   }
   app->OnAppCreateFrame(pWxui->m_pFrame);
   if (!pWxui->m_pFrame)
    break;
   pWxui->m_pFrame->Show(\
    dynamic_cast<Config*>(pWxui->ConfigGet())->\
    MainWindowInitialShow());
#else
   dynamic_cast<Skin*>(pWxui->SkinGet())->Perform();
#endif
   void(::SetEvent(hCreateEvent));
   wxTheApp->OnRun();
   wxTheApp->OnExit();
   wxEntryCleanup();
   pWxui->OnDestroy();
  } while (0);

  if (hCreateEvent)
   void(::SetEvent(hCreateEvent));
  return 0;
 }



#endif



 extern HINSTANCE __gpHinstance = nullptr;
}///namespace local