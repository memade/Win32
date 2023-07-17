#if !defined(__6FFA84F0_C910_4B24_B1BC_868BE616024B__)
#define __6FFA84F0_C910_4B24_B1BC_868BE616024B__

namespace shared {

 enum class CmdLineRuleType : unsigned long {
  Default = 0x00000000,
  Parent = 0x00000001,
  Identify = 0x00000002,
  MemadePrivatePacket = 0x00000003,

  Begin = Default,
  End = MemadePrivatePacket,
 };

 static /*const*/ std::map<CmdLineRuleType, std::wstring> CmdLineRuleString = {
 {CmdLineRuleType::MemadePrivatePacket,L"--memade-private-packet"},
 {CmdLineRuleType::Parent,L"--parent"},
 {CmdLineRuleType::Identify,L"--identify"},
 };

 template<typename TRuleType>
 class CmdLine final {
 public:
  inline CmdLine(
   const std::map<TRuleType, std::wstring>& input_rule_map,
   const bool& bEncryption);
  inline void operator<<(const std::wstring&);
 private:
  inline ~CmdLine();
  inline void Parser(const std::wstring&, std::map<std::wstring, std::wstring>&) const;
 public:
  inline void Release() const;
  inline std::wstring GetValue(const TRuleType&) const;
 protected:
  const bool m_bEncryption;
  std::map<std::wstring, std::wstring> parser_cmd_map_;
  std::map<TRuleType, std::wstring> rule_map_;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

 template<typename TRuleType>
 inline void CmdLine<TRuleType>::operator<<(const std::wstring& input_cmdline_string) {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   if (input_cmdline_string.empty())
    break;
   std::wstring final_cmdline_string = input_cmdline_string;
   Parser(input_cmdline_string, parser_cmd_map_);
   if (parser_cmd_map_.empty())
    break;
   if (m_bEncryption) {
    std::string decode_cmdline_string = shared::Win::Encryption::base64_decode(\
     shared::IConv::WStringToMBytes(\
      parser_cmd_map_[CmdLineRuleString[CmdLineRuleType::MemadePrivatePacket]]));
    final_cmdline_string = shared::IConv::MBytesToWString(decode_cmdline_string);
   }
   Parser(final_cmdline_string, parser_cmd_map_);
  } while (0);
 }

 template<typename TRuleType>
 inline CmdLine<TRuleType>::CmdLine(
  const std::map<TRuleType, std::wstring>& input_rule_map,
  const bool& bEncryption)
  : rule_map_(input_rule_map)
  , m_bEncryption(bEncryption) {

 }

 template<typename TRuleType>
 inline CmdLine<TRuleType>::~CmdLine() {

 }

 template<typename TRuleType>
 inline void CmdLine<TRuleType>::Release() const { delete this; }

 template<typename TRuleType>
 inline std::wstring CmdLine<TRuleType>::GetValue(const TRuleType& key) const {
  std::wstring result;
  std::lock_guard<std::mutex>(*m_Mutex);
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

 template<typename TRuleType>
 inline void CmdLine<TRuleType>::Parser(\
  const std::wstring& input_cmd_string, std::map<std::wstring, std::wstring>& out_cmd_parser_map) const {
  out_cmd_parser_map.clear();
  do {
   if (input_cmd_string.empty())
    break;
   int nArgs = 0;
   LPWSTR* pArgs = nullptr;
   pArgs = ::CommandLineToArgvW(input_cmd_string.c_str(), &nArgs);
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
    /*
    This is the default command line parameter of the Windows system, which is generally the full path name of the current executable file*/
    if (i == 0 && shared::Win::AccessW(arg))
     continue;
    std::vector<std::wstring> spilts = shared::Win::StringSpiltW(arg, L"=");
    if (spilts.empty())
     continue;
    out_cmd_parser_map.emplace(spilts[0], spilts.size() > 1 ? spilts[1] : L"");
   }

   //!@ Clearup
   for (auto it = out_cmd_parser_map.begin(); it != out_cmd_parser_map.end();) {
    bool exists = false;
    for (const auto& node : rule_map_) {
     if (node.second.compare(it->first) == 0) {
      exists = true;
      break;
     }
    }
    if (!exists)
     it = out_cmd_parser_map.erase(it);
    else
     ++it;
   }
  } while (0);
 }




 using TypeCmdLine = CmdLine<CmdLineRuleType>;
}///namespace shared



/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 18 Apr 2023 00:20:18 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6FFA84F0_C910_4B24_B1BC_868BE616024B__