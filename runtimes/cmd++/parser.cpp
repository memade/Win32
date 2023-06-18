#include "stdafx.h"

namespace local {

 Parser::Parser() {
  Init();
 }
 Parser::~Parser() {
  UnInit();
 }
 void Parser::Release() const {
  delete this;
 }
 void Parser::Init() {
  m_pWxParser = new wxCmdLineParser();
 }
 void Parser::UnInit() {
  SK_DELETE_PTR(m_pWxParser);
 }

 bool Parser::Ready() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Ready.load();
 }
 void Parser::SetCmdLine(const char* cmdline) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pWxParser->SetCmdLine(wxEmptyString);
  m_CommandLineText.Clear();
  if (cmdline) {
   do {
    int argn = 0;
    auto args = ::CommandLineToArgvW(shared::IConv::MBytesToWString(cmdline).c_str(), &argn);
    if (!args || argn<=0)
     break;
    int i = shared::Win::AccessW(args[0]) ? 1 : 0;
    for (; i < argn; ++i) {
     m_CommandLineText.Append(args[i]).Append(" ");
    }
    m_pWxParser->SetCmdLine(m_CommandLineText);
   } while (0);
  }
 }
 bool Parser::Perform() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Ready.store(false);
  do {
   if (!m_pWxParser || m_CommandLineText.empty())
    break;
   if (0 != m_pWxParser->Parse())
    break;
   m_Ready.store(true);
  } while (0);
  return m_Ready.load();
 }
 void Parser::SetSwitchChars(const char* value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (value)
   m_pWxParser->SetSwitchChars(wxString(value));
 }
 void Parser::SetLogo(const char* value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (value)
   m_pWxParser->SetLogo(wxString(value));
 }
 void Parser::AddSwitch(const char* name) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (name)
   m_pWxParser->AddSwitch(wxString(name));
 }
 void Parser::AddLongSwitch(const char* name) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (name)
   m_pWxParser->AddLongSwitch(wxString(name));
 }
 void Parser::AddOption(const char* name, const char* value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (name)
   m_pWxParser->AddOption(wxString(name), value ? wxString(value) : wxString(""));
 }
 void Parser::AddLongOption(const char* name, const char* value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (name)
   m_pWxParser->AddLongOption(wxString(name), value ? wxString(value) : wxString(""));
 }
 void Parser::AddParam(const char* value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (value)
   m_pWxParser->AddParam(wxString(value));
 }
 void Parser::GetParam(const size_t& index, const tfOnAllcoStringCb& result_cb) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  wxString result = m_pWxParser->GetParam(index);
  if (!result.IsEmpty()) {
   std::string std_string_result = result.ToStdString();
   result_cb(std_string_result.data());
  }
 }
 bool Parser::Found(const char* name) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!name)
    break;
   result = m_pWxParser->Found(wxString(name));
  } while (0);
  return result;
 }
 bool Parser::FoundSwitch(const char* name) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!name)
    break;
   result = m_pWxParser->FoundSwitch(wxString(name)) == wxCmdLineSwitchState::wxCMD_SWITCH_ON;
  } while (0);
  return result;
 }
 bool Parser::Found(const char* name, const tfOnAllcoStringCb& result_cb) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!name)
    break;
   wxString found_result;
   if (!m_pWxParser->Found(wxString(name), &found_result))
    break;
   std::string std_string_result = found_result.ToStdString();
   result_cb(std_string_result.data());
   result = true;
  } while (0);
  return result;
 }
 bool Parser::Found(const char* name, long& value) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!name)
    break;
   if (!m_pWxParser->Found(wxString(name), &value))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Parser::Found(const char* name, double& value) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!name)
    break;
   if (!m_pWxParser->Found(wxString(name), &value))
    break;
   result = true;
  } while (0);
  return result;
 }

}///namespace local