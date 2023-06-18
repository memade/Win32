#include "stdafx.h"

namespace local {

 Cmd::Cmd() {
  Init();
 }

 Cmd::~Cmd() {
  UnInit();
 }
 void Cmd::Release() const {
  delete this;
 }
 IParser* Cmd::ParserCreate() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return dynamic_cast<IParser*>(*m_Parsers.emplace(new Parser()).first);
 }
 bool Cmd::ParserDestory(IParser*& parser) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (!parser)
   return result;
  auto found = m_Parsers.find(dynamic_cast<Parser*>(parser));
  if (found != m_Parsers.end()) {
   (*found)->Release();
   m_Parsers.erase(found);
   result = true;
  }
  return result;
 }

#if 0
 const wxString g_cmdLineDesc[] =
 {
     wxT("Options:"),
     wxT("  -v, --version     Displays the version number"),
     wxT("  -n, --name        Specifies a name"),
     wxT("  -a, --age         Specifies an age"),
     wxT(""),
     wxT("Examples:"),
     wxT("  MyApp -v                    Displays the version number"),
     wxT("  MyApp -n John -a 30         Specifies name as John and age as 30"),
     wxT(""),
 };

 //wxCmdLineParser parser(__argc, __argv);
 wxCmdLineParser parser("-identify -a 220220");
 //parser.AddSwitch("v", "version", "Displays the version number");
 //parser.AddOption("n", "name", "Specifies a name", wxCMD_LINE_VAL_NUMBER);
 //parser.AddOption("a", "age", "Specifies an age", wxCMD_LINE_VAL_STRING);
 parser.AddSwitch("identify", "identify", "process identify.");
 parser.AddOption("a", "age", "Specifies an age", wxCMD_LINE_VAL_NUMBER);

 //parser.SetDesc(g_cmdLineDesc);
 parser.SetSwitchChars("-");
 parser.SetLogo(wxString::Format("Welcome to MyApp %s", "1.0"));

 if (parser.Parse() != 0)
  return false;

 bool showVersion = parser.FoundSwitch("identify");
 if (showVersion)
 {
  long age = 0;
  parser.Found("a", &age);


  auto xx = 0;
 }

 wxString name;
 parser.Found("n", &name);
 long age = 0;
 parser.Found("a", &age);
#endif

 void Cmd::Init() {
  std::string intpu_cmdline;
  for (int i = 0; i < __argc; ++i) {
   auto view = __argv[i];
   if (shared::Win::AccessA(view))
    continue;
   intpu_cmdline.append(view).append(" ");
  }
 



  //wxCmdLineParser parser(cmdline_string);

  //do {

  // parser.AddSwitch("identify", "identify", "process identify.");
  // parser.AddOption("a", "age", "Specifies an age", wxCMD_LINE_VAL_NUMBER);
  // parser.SetSwitchChars("-");
  // parser.SetLogo(wxString::Format("Welcome to MyApp %s", "1.0"));

  // if (parser.Parse() != 0)
  //  break;


  // if (wxCmdLineSwitchState::wxCMD_SWITCH_ON == parser.FoundSwitch("identify")) {


  //  auto sksksk = 0;
  // }


  // auto sk = 0;
  //} while (0);

 }

 void Cmd::UnInit() {
  for (auto& parser : m_Parsers) {
   parser->Release();

  }
 }

 extern HINSTANCE __gpHinstance = nullptr;
}///namespace local