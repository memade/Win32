#include "stdafx.h"

namespace wx {
 IWxui::IWxui(const HINSTANCE& hinstance) : m_hInstance(hinstance) {

 }
 IWxui::~IWxui() {

 }
 void IWxui::Release() const {
  delete this;
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
   //::SendMessageW(GlobalWindowConfigGet()->hMain, WM_CLOSE, 0, 0);
   //::WaitForSingleObject(m_hUIMain, INFINITE);
   //SK_DELETE_PTR(m_pTheme1);
   //SK_DELETE_PTR(m_pTheme2);
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
     frame->SetSize(100, 100);
     m_pTheme1 = new wx::Theme("0", R"(C:\Users\k34ub\Desktop\theme\2\shape.png)", R"(C:\Users\k34ub\Desktop\theme\2\bgk.png)");
     m_pTheme2 = new wx::Theme("1", R"(C:\Users\k34ub\Desktop\theme\GodOfWealth.png)", R"(C:\Users\k34ub\Desktop\theme\GodOfWealth.png)");

     auto theFrame = wxDynamicCast(frame, wx::IwxFrameSkin);
     theFrame->AppendTheme(m_pTheme1);
     theFrame->AppendTheme(m_pTheme2);
     theFrame->SetTheme();
     theFrame->Center();
     //frame->SetBackgroundColour(wxColour(RGB(0, 0, 0)));
     NotifyMainCreateEvent();
    });
   auto frame = new wx::IwxFrameSkin(nullptr);
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
   ::CloseHandle(m_hUIMainCreateEvent);
   m_hUIMainCreateEvent = nullptr;
   result = true;
  } while (0);
  return result;
 }
}///namespace wx