#include <win.hpp>

#if 1
#include <imui.hpp>
using namespace imui;
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pImui = IImui::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\imui++.dll)");
 auto config = pImui->ConfigGet();
 config->MainWindowInitialShow(false);
 pImui->Start();

 pImui->Show(true);

 Sleep(5000);

 pImui->Show(false);

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    pImui->Stop();
    IImui::DestoryInterface(pImui);
    exit = true;
   }

  });
 return 0;
}

#else
#include <imgui.hpp>
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 ImGui::ImguiBase* imgui = new ImGui::ImguiBase();

 imgui->Start();

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    imgui->Stop();
    exit = true;
   }

  });

 SK_DELETE_PTR(imgui);
 return 0;
}
#endif