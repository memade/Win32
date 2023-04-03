#include <win.hpp>
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