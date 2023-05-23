#include <win.hpp>
#include <libuv.hpp>
using namespace libuv;

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
 client->RegisterOnHookConnectionCb(
  [](std::string& address) {
  
   auto sk = 0;
  });
 client->RegisterOnConnectionCb(
  [](auto session) {  
   auto sss = 0;
  });
 client->RegisterOnDisconnectionCb(
  [](auto session) {
  
   auto sss = 0;
  });
 client->RegisterOnHookSystemExitCb([](bool& close) {
  auto sksks = 0;
  });
 client->RegisterOnSystemExitCb(
  []() { 
   auto sksksk = 0;
  });

 client->RegisterOnMessageCb([](const ISession* session, const CommandType& cmd, const std::string& message) {
  
   auto sksks = 0;
  });
 client->RegisterOnWelcomeCb(
  [](const ISession* pSession, const std::string& message_receive, std::string& message_reply) {
  
   auto sksksk = 0;
  });
 client->RegisterOnReceiveReplyCb([](const ISession* session, \
  const CommandType& cmd_receive, const std::string& message_receive,
  CommandType& cmd_reply,
  std::string& message_reply) {
  
   auto sksks = 0;
  });
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