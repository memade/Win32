#include <win.hpp>
#include <libuv.hpp>
using namespace libuv;


int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif
 auto server = IService::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\uv++.dll)",
  nullptr,
  static_cast<unsigned long>(libuv::ServerType::ACCEPTOR) | 
  static_cast<unsigned long>(AddressType::IPC) | 
  static_cast<unsigned long>(SessionType::IPC));
 auto config = server->ConfigGet();
 //config->Address(R"(0.0.0.0:8888)");
 //config->Address(R"([0:0:0:0:0:0:0:0]:8888)");
 //config->Address(R"([fe80::ce60:ae75:e7a:4af2%4]:8888)");
 config->Address(R"(\\.\pipe\server)");

 server->RegisterOnHookSessionCreateCb(
  [](const ISession* pSession) {

   auto sk = 0;
  });
 server->RegisterOnHookSessionDestoryCb(
  [](const ISession* pSession) {

   auto sk = 0;
  });
 server->RegisterOnServerReadyCb(
  []() {
   auto sk = 0;
  });
 server->RegisterOnHookSystemExitCb(
  [](bool& is_close) {
   auto sksks = 0;
  });
 server->RegisterOnSystemExitCb(
  []() {
   auto sksks = 0;
  });
 server->RegisterOnHookWelcomeSendCb(
  [](const ISession* pSession, const ICallback::tfOnAllcoStringCb& message_reply_cb) {

   auto sk = 0;
  });
 server->RegisterOnMessageCb(
  [](const libuv::ISession* pSession, const libuv::CommandType& type, const std::string& message) {

   auto sk = 0;
  });
 server->RegisterOnReceiveReplyCb(
  [](const ISession* session, 
   const CommandType& cmd_receive, const std::string& message_receive,
   CommandType& cmd_reply,
   const ICallback::tfOnAllcoStringCb& message_reply_cb) {

    switch (cmd_receive) {
    case CommandType::TESTMSG: {
     std::cout << message_receive << std::endl;
    }break;
    default:
     break;
    }

  });
 server->Start();
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    server->Stop();
    IService::DestoryInterface(server);
    exit = true;
   }
  });
 return 0;
}