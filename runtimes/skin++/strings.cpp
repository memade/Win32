#include "stdafx.h"

namespace local {

 String::String() {

 }
 String::String(const std::string& input) {
  *this << input;
 }
 String::String(const std::wstring& input) {
  *this << input;
 }
 String::String(const std::u8string& input) {
  *this << input;
 }
 String::String(const char* input) {
  *this << std::string(input?input:"");
 }
 String::String(const wchar_t* input) {
  *this << std::wstring(input ? input : L"");
 }
 String::String(const char8_t* input) {
  *this << std::u8string(input ? input : u8"");
 }
 String::~String() {
  u8_.clear();
  str_.clear();
  wstr_.clear();
 }
 void String::Release() const {
  delete this;
 }
 bool String::compare(const wchar_t* input_wstr, const bool& i /*= false*/) const {
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
 bool String::compare(const char* input_str, const bool& i /*= false*/) const {
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
 bool String::compare(const String& obj, const bool& i /*= false*/) const {
  return compare(dynamic_cast<const IString*>(&obj), i);
 }
 bool String::compare(const IString* iobj, const bool& i /*= false*/) const {
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
 const char* String::str() const {
  return str_.c_str();
 }
 const wchar_t* String::wstr() const {
  return wstr_.c_str();
 }
 const char8_t* String::u8() const {
  return u8_.c_str();
 }
 bool String::empty() const {
  return u8_.empty() || str_.empty() || wstr_.empty();
 }
 void String::operator=(const String& obj) {
  u8_ = obj.u8_;
  str_ = obj.str_;
  wstr_ = obj.wstr_;
 }
 void String::operator=(const std::string& str) {
  *this = String(str);
 }
 void String::operator=(const std::wstring& wstr) {
  *this = String(wstr);
 }
 void String::operator=(const std::u8string& u8) {
  *this = String(u8);
 }

 void String::operator<<(const std::u8string& u8str) {
  u8_ = u8str;
  wstr_ = shared::IConv::U8StringToWString(u8str);
  str_ = shared::IConv::U8StringToString(u8str);
 }
 void String::operator<<(const std::string& str) {
  str_ = str;
  wstr_ = shared::IConv::MBytesToWString(str);
  u8_ = shared::IConv::StringToU8String(str);
 }
 void String::operator<<(const std::wstring& wstr) {
  wstr_ = wstr;
  str_ = shared::IConv::WStringToMBytes(wstr);
  u8_ = shared::IConv::WStringToU8String(wstr);
 }
 const std::string& String::toString() const {
  return str_;
 }
 const std::wstring& String::toStringW() const {
  return wstr_;
 }
 const std::u8string& String::toStringU8() const {
  return u8_;
 }
 bool String::operator<(const String& obj) const {
  if (u8_.compare(obj.u8_) < 0 && wstr_.compare(obj.wstr_) < 0 && str_.compare(obj.str_) < 0)
   return true;
  return false;
 }
 bool String::operator>(const String& obj) const {
  if (u8_.compare(obj.u8_) > 0 && wstr_.compare(obj.wstr_) > 0 && str_.compare(obj.str_) > 0)
   return true;
  return false;
 }
 bool String::operator!=(const String& obj) const {
  return !(*this == obj);
 }
 bool String::operator==(const String& obj) const {
  if (u8_.compare(obj.u8_) == 0 && wstr_.compare(obj.wstr_) == 0 && str_.compare(obj.str_) == 0)
   return true;
  return false;
 }
 String String::operator+(const String& obj) const {
  String result;
  result.u8_ = u8_ + obj.u8_;
  result.str_ = str_ + obj.str_;
  result.wstr_ = wstr_ + obj.wstr_;
  return result;
 }
 String String::operator+(const std::string& obj) const {
  return *this + String(obj);
 }
 String String::operator+(const std::u8string& obj) const {
  return *this + String(obj);
 }
 String String::operator+(const std::wstring& obj) const {
  return *this + String(obj);
 }
 String& String::operator+=(const String& obj) {
  u8_ += obj.u8_;
  str_ += obj.str_;
  wstr_ += obj.wstr_;
  return *this;
 }
 String& String::operator+=(const std::string& obj) {
  *this += String(obj);
  return *this;
 }
 String& String::operator+=(const std::u8string& obj) {
  *this += String(obj);
  return *this;
 }
 String& String::operator+=(const std::wstring& obj) {
  *this += String(obj);
  return *this;
 }
}///namespace local