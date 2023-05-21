#include <win.hpp>
#include <libuv.hpp>

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif
 auto client = libuv::IClient::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\uv++.dll)", nullptr, static_cast<unsigned long>(libuv::ServerType::INITIATOR));
 auto config = client->ConfigGet();
#if 0
 config->Session(libuv::SessionType::TCP);
 config->IP(libuv::IPType::IPPROTO_IPV4);
 config->Address(R"(127.0.0.1:8888)");
#else
 config->Session(libuv::SessionType::IPC);
 config->Address(R"(\\?\pipe\server)");
 //config->Session(libuv::SessionType::UDP);
 //config->IP(libuv::IPType::IPPROTO_IPV4);
 //config->Address(R"(127.0.0.1:8888)");
#endif
 client->Start();

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    client->Stop();
    libuv::IClient::DestoryInterface(client);
    exit = true;
   }
  });
 return 0;
}