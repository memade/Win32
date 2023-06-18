#if !defined(__6FFA84F0_C910_4B24_B1BC_868BE616024B__)
#define __6FFA84F0_C910_4B24_B1BC_868BE616024B__

namespace shared {

 template<typename TRuleType, typename TRuleStringType = std::wstring, typename CmdLineStringType = std::wstring>
 class CmdLine final {
 public:
  CmdLine(const std::map<TRuleType, TRuleStringType>& input_rule_map,
   const CmdLineStringType& input_cmd_string,
   const bool& bEncryption)
   : rule_map_(input_rule_map)
   , cmd_string_(input_cmd_string)
   , m_bEncryption(bEncryption) {
   Init();
  }
  ~CmdLine() {
   UnInit();
  }
  void Release() const { delete this; }
 public:
  CmdLineStringType GetValue(const TRuleType& key) const {
   CmdLineStringType result;
   std::lock_guard<std::mutex> lock{*m_Mutex};
   do {
    auto found = rule_map_.find(key);
    if (found == rule_map_.end())
     break;
    auto found_node = parser_cmd_map_.find(found->second);
    if (found_node == parser_cmd_map_.end())
     break;
    result = found_node->second;
   } while (0);
   return result;
  }
 protected:
  const bool m_bEncryption = false;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
  inline void Init();
  inline void UnInit();
  std::map<CmdLineStringType, CmdLineStringType> parser_cmd_map_;
  const CmdLineStringType cmd_string_;
  const std::map<TRuleType, TRuleStringType> rule_map_;
 };



 template<typename TRuleType, typename TRuleStringType, typename CmdLineStringType>
 inline void CmdLine<TRuleType, TRuleStringType, CmdLineStringType>::Init() {
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
     /*parser_cmd_map_.emplace(rule_map_[CmdTag::IMAGE_PATHNAME], arg);*/
     continue;
    }
    std::vector<std::wstring> spilts = shared::Win::StringSpiltW(arg, L"=");
    if (spilts.empty())
     continue;
    parser_cmd_map_.emplace(spilts[0], spilts.size() > 1 ? spilts[1] : L"");
   }

#if 0
   auto found_image_pathname = parser_cmd_map_.find(rule_map_[CmdTag::IMAGE_PATHNAME]);
   if (found_image_pathname == parser_cmd_map_.end()) {
    parser_cmd_map_.emplace(rule_map_[CmdTag::IMAGE_PATHNAME], shared::Win::GetModulePathnameW());
   }

   auto found_type = parser_cmd_map_.find(rule_map_[CmdTag::TYPE]);
   if (found_type == parser_cmd_map_.end()) {
    parser_cmd_map_.emplace(rule_map_[CmdTag::TYPE], L"1");
   }
#endif

   //!@ Clearup
   for (auto it = parser_cmd_map_.begin(); it != parser_cmd_map_.end();) {

    bool exists = false;
    for (const auto& node : rule_map_) {
     if (node.second.compare(it->first) == 0) {
      exists = true;
      break;
     }
    }
    if (!exists)
     it = parser_cmd_map_.erase(it);
    else
     ++it;
   }

  } while (0);
 }

 template<typename TRuleType, typename TRuleStringType, typename CmdLineStringType>
 inline void CmdLine<TRuleType, TRuleStringType, CmdLineStringType>::UnInit() {
  parser_cmd_map_.clear();
 }

#if 0
 inline std::wstring Cmdline::GetValue(const CmdTag& tag) const {
  std::wstring result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (tag<CmdTag::BEGIN || tag>CmdTag::END)
    break;
   auto found = parser_cmd_map_.find(CmdTagString[tag]);
   if (found == parser_cmd_map_.end())
    break;
   result = found->second;
  } while (0);
  return result;
 }
 inline std::wstring Cmdline::GetValue(const std::wstring& key) const {
  std::wstring result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   auto found = parser_cmd_map_.find(key);
   if (found == parser_cmd_map_.end())
    break;
   result = found->second;
  } while (0);
  return result;
 }
#endif
}///namespace shared



/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 18 Apr 2023 00:20:18 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6FFA84F0_C910_4B24_B1BC_868BE616024B__