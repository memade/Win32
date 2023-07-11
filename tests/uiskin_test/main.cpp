#include <win.hpp>

#define ENABLE_WXUI 0
#define ENABLE_IMUI 1

#if ENABLE_WXUI
#include <wxui.hpp>
#endif
#if ENABLE_IMUI
#include <imui.hpp>
#endif
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

#if ENABLE_IMUI
 skin::ISkinUI* pDearImGui = imui::IDearImGui::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\imui++.dll)");
 pDearImGui->SkinCreate(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\skin\main.xml)");
 pDearImGui->Start();
#endif

#if ENABLE_WXUI
 skin::ISkinUI* pWxui = wxui::IWxui::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\wxui++.dll)");
 pWxui->SkinCreate(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\skin\main.xml)");
 pWxui->Start();
#endif


 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
#if ENABLE_IMUI
    pDearImGui->Stop();
    auto p = dynamic_cast<imui::IDearImGui*>(pDearImGui);
    imui::IDearImGui::DestoryInterface(p);
#endif

#if ENABLE_WXUI
    pWxui->Stop();
    auto p = dynamic_cast<wxui::IWxui*>(pWxui);
    wxui::IWxui::DestoryInterface(p);
#endif
    exit = true;
   }

  });


 return 0;
}



