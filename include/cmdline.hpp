#if !defined(__6FFA84F0_C910_4B24_B1BC_868BE616024B__)
#define __6FFA84F0_C910_4B24_B1BC_868BE616024B__

namespace cmd {

 enum class EnProcessType : unsigned long long {
  PROCESS_TYPE_MAIN = 0,
  PROCESS_TYPE_CEF = 1,
  PROCESS_TYPE_IMGUI = 2,
  PROCESS_TYPE_DUI = 3,
  PROCESS_TYPE_UNKNOWN = 4,

  PROCESS_TYPE_BEGIN = PROCESS_TYPE_MAIN,
  PROCESS_TYPE_END = PROCESS_TYPE_DUI,

  PROCESS_TYPE_DEFAULT = PROCESS_TYPE_DUI,
 };

 const wchar_t cmd_flag_process_type[] = L"process_type";


 class Cmdline final {
 public:
  Cmdline(LPWSTR input_cmd_string) :m_cmd_string(input_cmd_string ? shared::IConv::ToLowerW(input_cmd_string) : L"") {
   Init();
  }
  ~Cmdline() {
   UnInit();
  }
 public:
  inline std::wstring GetValue(const std::wstring&) const;
  inline const EnProcessType& ProcessType() const;
  inline const std::wstring& CurrentModulePathname() const;
 private:
  inline void Init();
  inline void UnInit();
 private:
  std::wstring m_cmd_string;
  std::map<std::wstring, std::wstring> m_cmd_map;
  inline bool VerifyProcessType() const;
  EnProcessType m_ProcessType = EnProcessType::PROCESS_TYPE_MAIN;
  std::wstring m_CurrentModulePathname;
 };

 inline void Cmdline::Init() {
  do {
   if (m_cmd_string.empty())
    break;
   int nArgs = 0;
   LPWSTR* pArgs = nullptr;
   pArgs = ::CommandLineToArgvW(m_cmd_string.c_str(), &nArgs);
   if (!*pArgs)
    break;
   for (int i = 0; i < nArgs; i++) {
    if (i == 0) {
     m_CurrentModulePathname = pArgs[i];
     continue;
    }
    std::vector<std::wstring> spilts = shared::Win::StringSpiltW(pArgs[i], L"=");
    if (spilts.empty())
     continue;
    m_cmd_map.emplace(spilts[0], spilts.size() > 1 ? spilts[1] : L"");
   }

   m_ProcessType = EnProcessType(::StrToIntW(m_cmd_map[cmd_flag_process_type].c_str()));
   if (!VerifyProcessType())
    m_ProcessType = EnProcessType::PROCESS_TYPE_DEFAULT;

  } while (0);
 }
 inline void Cmdline::UnInit() {

 }
 inline std::wstring Cmdline::GetValue(const std::wstring& key) const {
  std::wstring result;
  do {
   auto found = m_cmd_map.find(key);
   if (found == m_cmd_map.end())
    break;
   result = found->second;
  } while (0);
  return result;
 }
 inline const EnProcessType& Cmdline::ProcessType() const {
  return m_ProcessType;
 }
 inline const std::wstring& Cmdline::CurrentModulePathname() const {
  return m_CurrentModulePathname;
 }
 inline bool Cmdline::VerifyProcessType() const {
  return m_ProcessType >= EnProcessType::PROCESS_TYPE_BEGIN && m_ProcessType <= EnProcessType::PROCESS_TYPE_END;
 }
}///namespace cmd



/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 18 Apr 2023 00:20:18 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6FFA84F0_C910_4B24_B1BC_868BE616024B__