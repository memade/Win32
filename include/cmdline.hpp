#if !defined(__6FFA84F0_C910_4B24_B1BC_868BE616024B__)
#define __6FFA84F0_C910_4B24_B1BC_868BE616024B__

namespace shared {

 enum class ProcessType : unsigned long long {
  UNKNOWN = 0,
  MAIN = 1,
  CHILD_TEST,
#if 0
  PROCESS_TYPE_MAIN = 0,
  PROCESS_TYPE_CEF = 1,
  PROCESS_TYPE_IMGUI = 2,
  PROCESS_TYPE_DUI = 3,
  PROCESS_TYPE_UNKNOWN = 4,

  PROCESS_TYPE_BEGIN = PROCESS_TYPE_MAIN,
  PROCESS_TYPE_END = PROCESS_TYPE_DUI,

  PROCESS_TYPE_DEFAULT = PROCESS_TYPE_DUI,
#endif
  BEGIN = MAIN,
  END = CHILD_TEST,
 };

 enum class CmdTag : unsigned long {
  IMAGE_PATHNAME = 0,
  TYPE = 1,/*process type*/
  TCP_SERVER,/*process server tcp = address*/
  TCP_CLIENT,/*process client tcp = address*/
  IPC_SERVER,
  IPC_CLIENT,
  UDP_SERVER,
  UDP_CLIENT,

  BEGIN = IMAGE_PATHNAME,
  END = UDP_CLIENT,
 };


 static /*const*/ std::map<CmdTag, std::wstring> CmdTagString = {
  {CmdTag::IMAGE_PATHNAME,L"--image-pathname"},
  {CmdTag::TYPE,L"--type"},
  {CmdTag::TCP_SERVER,L"--tcp-server"},
  {CmdTag::TCP_CLIENT,L"--tcp-client"},
  {CmdTag::IPC_SERVER,L"--ipc-server"},
  {CmdTag::IPC_CLIENT,L"--ipc-client"},
  {CmdTag::UDP_SERVER,L"--udp-server"},
  {CmdTag::UDP_CLIENT,L"--udp-client"},
 };


 class Cmdline final {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Cmdline(LPCWSTR input_cmd_string)
   : cmd_string_(input_cmd_string ? shared::IConv::ToLowerW(input_cmd_string) : L"") {
   Init();
  }
  ~Cmdline() {
   UnInit();
  }
 public:
  inline std::wstring GetValue(const std::wstring&) const;
  inline std::wstring GetValue(const CmdTag&) const;
  inline ProcessType GetType() const;
 private:
  inline void Init();
  inline void UnInit();
 private:
  std::map<std::wstring, std::wstring> cmd_map_;
  const std::wstring cmd_string_;
 };

 inline void Cmdline::Init() {
  do {
   if (cmd_string_.empty())
    break;
   int nArgs = 0;
   LPWSTR* pArgs = nullptr;
   pArgs = ::CommandLineToArgvW(cmd_string_.c_str(), &nArgs);
   if (!*pArgs)
    break;
   for (int i = 0; i < nArgs; i++) {
    std::wstring arg = pArgs[i];
    for (auto it = arg.begin(); it != arg.end();) {
     if (*it == L'\n' || *it == L'\r')
      it = arg.erase(it);
     else
      ++it;
    }

    if (i == 0 && shared::Win::AccessW(arg)) {
     cmd_map_.emplace(CmdTagString[CmdTag::IMAGE_PATHNAME], arg);
     continue;
    }
    std::vector<std::wstring> spilts = shared::Win::StringSpiltW(arg, L"=");
    if (spilts.empty())
     continue;
    cmd_map_.emplace(spilts[0], spilts.size() > 1 ? spilts[1] : L"");
   }

   auto found_image_pathname = cmd_map_.find(CmdTagString[CmdTag::IMAGE_PATHNAME]);
   if (found_image_pathname == cmd_map_.end()) {
    cmd_map_.emplace(CmdTagString[CmdTag::IMAGE_PATHNAME], shared::Win::GetModulePathnameW());
   }

   auto found_type = cmd_map_.find(CmdTagString[CmdTag::TYPE]);
   if (found_type == cmd_map_.end()) {
    cmd_map_.emplace(CmdTagString[CmdTag::TYPE], L"1");
   }

   //!@ Clearup
   for (auto it = cmd_map_.begin(); it != cmd_map_.end();) {

    bool exists = false;
    for (const auto& node : CmdTagString) {
     if (node.second.compare(it->first) == 0) {
      exists = true;
      break;
     }
    }
    if (!exists)
     it = cmd_map_.erase(it);
    else
     ++it;
   }

  } while (0);
 }
 inline void Cmdline::UnInit() {

 }
 inline ProcessType Cmdline::GetType() const {
  ProcessType result = ProcessType::MAIN;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   auto found = cmd_map_.find(CmdTagString[CmdTag::TYPE]);
   if (found == cmd_map_.end())
    break;
   result = static_cast<decltype(result)>(_wtol(found->second.c_str()));
  } while (0);
  return result;
 }
 inline std::wstring Cmdline::GetValue(const CmdTag& tag) const {
  std::wstring result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (tag<CmdTag::BEGIN || tag>CmdTag::END)
    break;
   auto found = cmd_map_.find(CmdTagString[tag]);
   if (found == cmd_map_.end())
    break;
   result = found->second;
  } while (0);
  return result;
 }
 inline std::wstring Cmdline::GetValue(const std::wstring& key) const {
  std::wstring result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   auto found = cmd_map_.find(key);
   if (found == cmd_map_.end())
    break;
   result = found->second;
  } while (0);
  return result;
 }
}///namespace shared



/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 18 Apr 2023 00:20:18 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6FFA84F0_C910_4B24_B1BC_868BE616024B__