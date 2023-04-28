#include <win.hpp>
#include <libuvpp.hpp>

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pLibuv = libuvpp::ILibuv::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\libuvpp.dll)");
 auto pServer = pLibuv->CreateServer();
 pServer->Release();
 //pServer->Start();

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    //pServer->Stop();
    libuvpp::ILibuv::DestoryInterface(pLibuv);
    exit = true;
   }
  });
 return 0;
}
