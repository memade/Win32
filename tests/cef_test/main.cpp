#include <win.hpp>
#include <cef.hpp>
using namespace cef;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pCef = cef::ICef::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\cef++.dll)");
 auto config = pCef->ConfigGet();
 pCef->Start();


 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    pCef->Stop();
    cef::ICef::DestoryInterface(pCef);
    exit = true;
   }

  });


 return 0;
}
