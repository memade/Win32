#include "stdafx.h"
using namespace cjson;

namespace local {

 Body::Body() {

 }
 Body::Body(const char* input) {
  if (input)
   m_json_root = cJSON_Parse(input);
 }
 Body::Body(const std::string& input) {
  if (!input.empty())
   m_json_root = cJSON_Parse(input.data());
 }
 Body::~Body() {
  SK_DELETE_PTR_C(m_json_root);
 }
 void Body::clear() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  SK_DELETE_PTR_C(m_json_root);
 }
 bool Body::empty() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return !m_json_root;
 }
 bool Body::operator<<(const std::string& input) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  SK_DELETE_PTR_C(m_json_root);
  do {
   if (input.empty())
    break;
   m_json_root = cJSON_Parse(input.data());
   if (!m_json_root)
    break;
   if (!cJSON_IsObject(m_json_root))
    break;

   result = true;
  } while (0);
  return result;
 }

 void Body::operator>>(std::string& output) const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  output.clear();
  if (m_json_root)
   output = cJSON_Print(m_json_root);
 }

}///namespace local
