#include "stdafx.h"

namespace wx {
 IWxui::IWxui(const HINSTANCE& hinstance) : m_hInstance(hinstance) {
  m_hUIMainCreateEvent = ::CreateEventW(NULL, TRUE, FALSE, NULL);
 }
 IWxui::~IWxui() {
  SK_CLOSE_HANDLE(m_hUIMainCreateEvent);
 }

 void IWxui::Release() const {
  delete this;
 }
 void IWxui::EnableExitConfirmation(const bool& enable) {
  m_EnableExitConfirmation.store(enable);
 }
 bool IWxui::Start() {
  do {
#if 0
   INITCOMMONCONTROLSEX icc = { 0 };
   icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icc.dwICC = ICC_WIN95_CLASSES;
   if (FALSE == ::InitCommonControlsEx(&icc))
    break;
#endif
   if (m_IsOpenUI.load())
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
   m_IsOpenUI.store(true);
  } while (0);
  return m_IsOpenUI.load();
 }
 void IWxui::Stop() {
  do {
   if (!m_IsOpenUI.load())
    break;
   ::SendMessageW(m_hWnd, WM_CLOSE, 0, 0);
   ::WaitForSingleObject(m_hUIMain, INFINITE);
   m_IsOpenUI.store(false);
  } while (0);
 }
 void IWxui::MainProcess() {
  do {
   if (!m_hInstance)
    break;
   wxDISABLE_DEBUG_SUPPORT();
   wxInitializer wxinit;
   if (!wxinit.IsOk())
    break;


   auto app = wxDynamicCast(wxApp::GetInstance(), IwxApp);
   app->RegisterAppCreateFrameEventCb(
    [&](wxFrame* frame) {
     auto mdiFrameWnd = wxDynamicCast(frame, IwxMDIParentFrame);
     mdiFrameWnd->EnableExitConfirmation(m_EnableExitConfirmation.load());
     m_hWnd = mdiFrameWnd->GetHwnd();
     mdiFrameWnd->Center();
     NotifyMainCreateEvent();
    });
   auto frame = new wx::IwxMDIParentFrame(nullptr);
   wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD, wx::WX_CMD_ONAPPCREATEFRAME);
   event->SetEventObject(frame);
   event->SetString("Hello wxWidgets!");
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