#include <win.hpp>
#include <wxcmd.hpp>
using namespace cmd;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pWxcmd = cmd::ICmd::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\cmd++.dll)");


 

#if 0
 auto parser_switch = pWxcmd->ParserCreate();
 parser_switch->SetCmdLine(R"(--identify --asflkg 220220)");
 parser_switch->AddLongSwitch("identify");
 parser_switch->AddLongOption("asflkg", "age");
 parser_switch->SetSwitchChars("-");
 parser_switch->Perform();
 parser_switch->FoundSwitch("identify");
 parser_switch->Found("asflkg",
  [](const char* value) {

   auto sksks = 0;
  });
 pWxcmd->ParserDestory(parser_switch);
#endif

#if 1
 cmd::IParser* parser_param = nullptr;
 do {
  parser_param = pWxcmd->ParserCreate();
  if (!parser_param)
   break;
  parser_param->SetCmdLine(R"(--identify)");
  parser_param->AddParam("identify");
  parser_param->SetSwitchChars("-");
  if (!parser_param->Perform())
   break;

  auto xx = 0;
 } while (0);
 pWxcmd->ParserDestory(parser_param);

#endif


 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    cmd::ICmd::DestoryInterface(pWxcmd);
    exit = true;
   }

  });


 return 0;
}

