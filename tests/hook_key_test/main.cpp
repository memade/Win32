#include <win.hpp>

HHOOK GHOOK = nullptr;
int _tmain(int argc, TCHAR** argv) {
#if defined(_DEBUG)
 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //_CrtSetBreakAlloc(1447633);
#endif

 //shared::Win::AdjustProcessOrThreadPrivilege(::GetCurrentProcess(), SE_DEBUG_NAME);
 //shared::Win::AdjustProcessOrThreadPrivilege(::GetCurrentProcess(), SE_TCB_NAME);

 GHOOK = ::SetWindowsHookEx(WH_KEYBOARD_LL,
  [](int nCode, WPARAM wParam, LPARAM lParam)->LRESULT {
   KBDLLHOOKSTRUCT* pKHook = (KBDLLHOOKSTRUCT*)lParam;
   if (pKHook) {
    if (HC_ACTION == nCode) {
     switch (wParam) {
     case WM_KEYDOWN:
      [[fallthrough]];
     case WM_SYSKEYDOWN: {
      if ('h' == pKHook->vkCode || 'H' == pKHook->vkCode) {
       if (((GetKeyState(VK_LCONTROL) & 0x8000) && (GetKeyState(VK_LMENU) & 0x8000)) || \
        ((GetKeyState(VK_RCONTROL) & 0x8000) && (GetKeyState(VK_RMENU) & 0x8000))) {
        //if (::IsWindowVisible(local::Global::ConsoleHwnd())) {
        //	::ShowWindow(local::Global::ConsoleHwnd(), SW_HIDE);
        //}
        //else {
        //	::ShowWindow(local::Global::ConsoleHwnd(), SW_SHOW);
        //}
        /*LOGWARN("{}", "CTRL + ATL + H");*/
        auto sk = 0;
       }
      }
      else if ('q' == pKHook->vkCode || 'Q' == pKHook->vkCode) {
       if (((GetKeyState(VK_LCONTROL) & 0x8000) && (GetKeyState(VK_LMENU) & 0x8000)) || \
        ((GetKeyState(VK_RCONTROL) & 0x8000) && (GetKeyState(VK_RMENU) & 0x8000))) {
        //	local::Global::SystemExit(true);
         /*LOGWARN("{}", "CTRL + ATL + Q");*/
        auto sk = 0;
       }
      }
     }break;
     }///switch
    }
   }
   return ::CallNextHookEx(GHOOK, nCode, wParam, lParam);
  }, NULL, 0);
 if (!GHOOK) {
  //LOGERROR("Hook system level layet keyboard failed({})", ::GetLastError());
 }


#if 1
 do {
  MSG msg = { 0 };
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
   if (msg.message == WM_QUERYENDSESSION ||
    msg.message == WM_CLOSE ||
    msg.message == WM_DESTROY ||
    msg.message == WM_QUIT ||
    msg.message == WM_ENDSESSION) {
    //local::Global::SystemExit(true);
   }
  }
  //if (local::Global::SystemExit()) {
  //	local::Global::CoreGet()->Close();
  //	sk::Helper::WriteConsoleInputQ();
  //	process.join();
  //	break;
  //}
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
 } while (1);
#else
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    exit = true;
   }

  });
#endif

 if (GHOOK) {
  ::UnhookWindowsHookEx(GHOOK);
 }

 return 0;
}