#include <win.hpp>
#include <libuv.hpp>

#define ENABLE_SERVER_TCP 0
#define ENABLE_CLIENT_TCP 0
#define ENABLE_SERVER_UDP 1
#define ENABLE_CLIENT_UDP 0
#define ENABLE_SERVER_IPC 0
#define ENABLE_CLIENT_IPC 0
#if 1
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif
 auto server = libuv::IServer::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\libuv++.dll)",
  nullptr,
#if ENABLE_SERVER_TCP
  static_cast<unsigned long>(libuv::SessionType::TCP_SESSION_SERVER),
#elif ENABLE_SERVER_UDP
  static_cast<unsigned long>(libuv::SessionType::UDP_SESSION_SERVER),
#elif ENABLE_SERVER_IPC
  static_cast<unsigned long>(libuv::SessionType::IPC_SESSION_SERVER),
#elif ENABLE_CLIENT_TCP
  static_cast<unsigned long>(libuv::SessionType::TCP_SESSION_CLIENT),
#elif ENABLE_CLIENT_UDP
  static_cast<unsigned long>(libuv::SessionType::UDP_SESSION_CLIENT),
#elif ENABLE_CLIENT_IPC
  static_cast<unsigned long>(libuv::SessionType::IPC_SESSION_CLIENT),
#endif
  "api_server_init",
  "api_server_uninit"
  );
 auto config = server->ConfigGet();
#if ENABLE_SERVER_TCP || ENABLE_SERVER_UDP
 config->IPAddrV4("0.0.0.0");
 config->Port(8888);
#endif

#if ENABLE_SERVER_IPC
 config->PipeName(R"(\\?\pipe\server)");
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
#else




#endif