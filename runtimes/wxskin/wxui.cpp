#include "stdafx.h"

namespace wx {
 IWxui::IWxui(const HINSTANCE& hinstance, const bool& show /*= true*/, const std::wstring& title)
  : m_hInstance(hinstance)
  , m_TitleText(title)
  , m_bShow(show) {                                                       
  m_hUIMainCreateEvent = ::CreateEventW(NULL, TRUE, FALSE, NULL);
 }
 IWxui::~IWxui() {
  SK_CLOSE_HANDLE(m_hUIMainCreateEvent);
 }

 void IWxui::Release() const {
  delete this;
 }
 HWND IWxui::GetMainWnd() const {
  return m_hWnd;
 }
 void IWxui::EnableExitConfirmation(const bool& enable) {
  m_EnableExitConfirmation = enable;
 }
 bool IWxui::Start() {
  do {
   INITCOMMONCONTROLSEX icc = { 0 };
   icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icc.dwICC = ICC_WIN95_CLASSES;
   if (FALSE == ::InitCommonControlsEx(&icc))
    break;
   if (m_IsOpenUI)
    break;
   if (!m_hInstance)
    break;
   m_hUIMain = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0,
    [](void* param)->UINT {
     IWxui* pThis = reinterpret_cast<IWxui*>(param);
     pThis->MainProcess();
     return 0;
    }, this, 0, NULL));
   ::WaitForSingleObject(m_hUIMainCreateEvent, INFINITE);
   m_IsOpenUI = true;
  } while (0);
  return m_IsOpenUI;
 }
 void IWxui::Stop() {
  do {
   if (!m_IsOpenUI)
    break;
   ::SendMessageW(m_hWnd, WM_CLOSE, 0, 0);
   ::WaitForSingleObject(m_hUIMain, INFINITE);
   m_IsOpenUI = false;
  } while (0);
 }
 void IWxui::Show(const bool& show) {
  m_bShow = show;
  wxThreadEvent* e = new wxThreadEvent(wxEVT_THREAD, WX_CMD_SHOWWINDOW);
  e->SetInt(show ? 1 : 0);
  wxQueueEvent(wxApp::GetInstance(), e);
 }
 // void IWxui::RegisterCreateChildCb(const tfAppCreateFrameChildInitCallback& cb) const {
 //#if 0
 //  wxThreadEvent* event = new IwxThreadEvent;
 //  event->SetId(WX_CMD_ONAPPCREATEFRAME);
 //  dynamic_cast<IwxThreadEvent*>(event)->RoutePtr(nullptr);
 //  wxQueueEvent(wxApp::GetInstance(), event);
 //#endif
 //  m_pApp->RegisterAppCreateFrameChildInitCb(cb);
 // }
 const HANDLE& IWxui::MainHandle() const {
  return m_hUIMain;
 }
 void IWxui::MainProcess() {
  do {
   if (!m_hInstance)
    break;
   wxDISABLE_DEBUG_SUPPORT();
   wxInitializer wxinit;
   if (!wxinit.IsOk())
    break;
   auto app = reinterpret_cast<IwxApp*>(wxApp::GetInstance());
   app->RegisterOnAppCreateFrameCb(
    [&](wxFrame* frame) {
     auto mdiFrameWnd = wxDynamicCast(frame, IwxMDIParentFrame);
     mdiFrameWnd->EnableExitConfirmation(m_EnableExitConfirmation);
     m_hWnd = mdiFrameWnd->GetHwnd();
     mdiFrameWnd->Center();
     NotifyMainCreateEvent();
    });
   IwxMDIParentFrame* frame = new IwxMDIParentFrame(nullptr);
   frame->Show(m_bShow);
   wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD, WX_CMD_ONAPPCREATEFRAME);
   event->SetEventObject(frame);
   event->SetString(m_TitleText);
   wxQueueEvent(wxApp::GetInstance(), event);
   wxEntry(m_hInstance);
  } while (0);
 }

 bool IWxui::NotifyMainCreateEvent() {
  bool result = false;
  do {
   if (!m_hUIMainCreateEvent)
    break;
   if (FALSE == ::SetEvent(m_hUIMainCreateEvent))
    break;
   SK_CLOSE_HANDLE(m_hUIMainCreateEvent);
   result = true;
  } while (0);
  return result;
 }






}///namespace wx