#include <win.hpp>
#include <libuv.hpp>
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto server = libuv::IServer::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\uv++.dll)",
  nullptr,
  static_cast<unsigned long>(libuv::ServerType::ACCEPTOR));
 auto config = server->ConfigGet();
#if 0
 config->IP(libuv::IPType::IPPROTO_IPV4);
 config->Address(R"(0.0.0.0:8888)");
 config->Session(libuv::SessionType::TCP);
#else
 config->Address(R"(\\?\pipe\server)");
 config->Session(libuv::SessionType::IPC);
 //config->IP(libuv::IPType::IPPROTO_IPV4);
 //config->Address(R"(0.0.0.0:8888)");
 //config->Session(libuv::SessionType::UDP);
#endif
 server->Start();
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    server->Stop();
    libuv::IServer::DestoryInterface(server);
    exit = true;
   }
  });
 return 0;
}