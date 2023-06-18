#include <win.hpp>
#include <libuv.hpp>
using namespace libuv;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif
 auto client = IService::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\uv++.dll)", nullptr,
  static_cast<unsigned long>(libuv::ServerType::INITIATOR) |
  static_cast<unsigned long>(AddressType::IPC) |
  static_cast<unsigned long>(SessionType::IPC));
 auto config = client->ConfigGet();
 config->EnableClientWaitForTheInitialConnectionAndReceiveResult(true);
 //config->Address(R"(127.0.0.1:8888)");
 //config->Address(R"([0:0:0:0:0:0:0:1]:8888)");
 //config->Address(R"([fe80::ce60:ae75:e7a:4af2%4]:8888)");
 config->Address(R"(\\.\pipe\server)");

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
  [](const ISession* pSession, const std::string& message_receive, const ICallback::tfOnAllcoStringCb& message_reply_cb) {

   auto sksksk = 0;
  });
 client->RegisterOnReceiveReplyCb([](const ISession* session, \
  const CommandType& cmd_receive, const std::string& message_receive,
  CommandType& cmd_reply,
  const ICallback::tfOnAllcoStringCb& message_reply_cb) {

   auto sksks = 0;
  });
 client->Start();

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    client->Stop();
    IService::DestoryInterface(client);
    exit = true;
   }
   else if (input == "test" || input == "t" || input=="T") {
    client->Write(CommandType::TESTMSG, "test msg.", 9);
   }
  });
 return 0;
}