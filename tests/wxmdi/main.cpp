#include <win.hpp>
#include <wxskin.hpp>

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto wxui = new wx::IWxui(::GetModuleHandleW(nullptr));
 wxui->EnableExitConfirmation(false);
 wxui->Start();


 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    wxui->Stop();
    wxui->Release();
    exit = true;
   }

  });
 return 0;
}