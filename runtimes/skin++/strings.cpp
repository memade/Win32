#include "stdafx.h"

namespace local {

 Strings::Strings() {

 }
 Strings::Strings(const std::string& input) {
  *this << input;
 }
 Strings::Strings(const std::wstring& input) {
  *this << input;
 }
 Strings::Strings(const std::u8string& input) {
  *this << input;
 }
 Strings::Strings::~Strings() {

 }
 bool Strings::compare(const wchar_t* input_wstr, const bool& i /*= false*/) const {
  bool result = false;
  if (!input_wstr)
   return false;
  if (i) {
   result = wcsicmp(wstr_.c_str(), input_wstr) == 0;
  }
  else {
   result = wcscmp(wstr_.c_str(), input_wstr) == 0;
  }
  return result;
 }
 bool Strings::compare(const char* input_str, const bool& i /*= false*/) const {
  bool result = false;
  if (!input_str)
   return false;
  if (i) {
   result = stricmp(str_.c_str(), input_str) == 0;
  }
  else {
   result = strcmp(str_.c_str(), input_str) == 0;
  }
  return result;
 }
 bool Strings::compare(const IStrings* iobj, const bool& i /*= false*/) const {
  bool result = false;
  if (!iobj)
   return result;
  if (i) {
   std::wstring obj1 = shared::IConv::ToLowerW(wstr_);
   std::wstring obj2 = shared::IConv::ToLowerW(iobj->wstr());
   result = obj1.compare(obj2) == 0;
  }
  else {
   result = wstr_.compare(iobj->wstr()) == 0;
  }
  return result;
 }
 const char* Strings::str() const {
  return str_.c_str();
 }
 const wchar_t* Strings::wstr() const {
  return wstr_.c_str();
 }
 const char8_t* Strings::u8() const {
  return u8_.c_str();
 }
 bool Strings::empty() const {
  return u8_.empty() || str_.empty() || wstr_.empty();
 }
 void Strings::operator=(const Strings& obj) {
  u8_ = obj.u8_;
  str_ = obj.str_;
  wstr_ = obj.wstr_;
 }
 void Strings::operator<<(const std::u8string& u8str) {
  u8_ = u8str;
  wstr_ = shared::IConv::U8StringToWString(u8str);
  str_ = shared::IConv::U8StringToString(u8str);
 }
 void Strings::operator<<(const std::string& str) {
  str_ = str;
  wstr_ = shared::IConv::MBytesToWString(str);
  u8_ = shared::IConv::StringToU8String(str);
 }
 void Strings::operator<<(const std::wstring& wstr) {
  wstr_ = wstr;
  str_ = shared::IConv::WStringToMBytes(wstr);
  u8_ = shared::IConv::WStringToU8String(wstr);
 }
 const std::string& Strings::toString() const {
  return str_;
 }
 const std::wstring& Strings::toStringW() const {
  return wstr_;
 }
 const std::u8string& Strings::toStringU8() const {
  return u8_;
 }
}///namespace local