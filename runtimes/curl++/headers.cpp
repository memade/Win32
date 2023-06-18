#include "stdafx.h"

namespace local {

 Headers::Headers() {

 }

 Headers::~Headers() {

 }
 std::string Headers::search(const std::string& key) const {
  std::string result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  auto found = m_Headers.find(shared::IConv::ToLowerA(key));
  if (found != m_Headers.end())
   result = found->second;
  return result;
 }
 void Headers::operator<<(const std::string& input) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (input.empty())
    break;
   m_Source.append(input);
   auto found_end = input.find("\r\n");
   if (found_end == std::string::npos)
    break;
   //!@ HTTP/1.1 200 OK
   size_t found = input.find(R"(HTTP/)");
   if (found != std::string::npos) {
    std::string node = input.substr(found, found_end);
    std::vector<std::string> parser_s = shared::Win::StringSpiltA(node, " ");
    m_Headers.emplace("def_begin", node);
    m_Headers.emplace("def_protocol", parser_s.size() > 0 ? parser_s[0] : "");
    m_Headers.emplace("def_response_code", parser_s.size() > 1 ? parser_s[1] : "");
    m_Headers.emplace("def_result", parser_s.size() > 2 ? parser_s[2] : "");
    break;
   }
   if (input.compare("\r\n") == 0) {
    break;
   }
   size_t found_spilt = input.find(": ");
   if (found_spilt == std::string::npos)
    break;
   const auto key = shared::IConv::ToLowerA(input.substr(0, found_spilt));
   auto value = input.substr(found_spilt + strlen(": "), input.find("\r\n"));
   value.resize(value.size() - strlen("\r\n"));
   m_Headers.emplace(key, value);
  } while (0);
 }
 void Headers::operator>>(std::list<std::string>& output) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  output.clear();
  for (const auto& node : m_Headers)
   output.push_back(std::format("{}: {}", node.first, node.second));
 }
 void Headers::operator>>(std::string& output) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  output.clear();
  if (m_Source.empty()) {
   for (const auto& node : m_Headers)
    m_Source.append(node.first).append(": ").append(node.second).append("\r\n");
  }
  output = m_Source;
 }
 void Headers::push(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (key.empty())
   return;
  std::string lower_key = shared::IConv::ToLowerA(key);
  auto found = m_Headers.find(lower_key);
  if (found != m_Headers.end())
   m_Headers.erase(found);
  m_Headers.emplace(lower_key, value);
 }
}///namespace local
