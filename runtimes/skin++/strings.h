#if !defined(__82970C26_58E9_4899_87EC_F2374D6547D8__)
#define __82970C26_58E9_4899_87EC_F2374D6547D8__

namespace local {

 class String final : public IString {
 public:
  String();
  String(const std::string&);
  String(const std::wstring&);
  String(const std::u8string&);
  String(const char*);
  String(const wchar_t*);
  String(const char8_t*);
  virtual ~String();
  void Release() const override final;
  void operator=(const String&);
  void operator=(const std::string&);
  void operator=(const std::wstring&);
  void operator=(const std::u8string&);
  void operator<<(const std::string&);
  void operator<<(const std::wstring&);
  void operator<<(const std::u8string&);
  bool operator<(const String&) const;
  bool operator>(const String&) const;
  bool operator!=(const String&) const;
  bool operator==(const String&) const;
  const std::string& toString() const;
  String operator+(const String&) const;
  String operator+(const std::string&) const;
  String operator+(const std::u8string&) const;
  String operator+(const std::wstring&) const;
  String& operator+=(const String&);
  String& operator+=(const std::string&);
  String& operator+=(const std::u8string&);
  String& operator+=(const std::wstring&);
  const std::wstring& toStringW() const;
  const std::u8string& toStringU8() const;
 public:
  const char* str() const override final;
  const wchar_t* wstr() const override final;
  const char8_t* u8() const override final;
  bool empty() const override final;
  bool compare(const wchar_t*, const bool& i = false) const override final;
  bool compare(const char*, const bool& i = false) const override final;
  bool compare(const IString*, const bool& i = false) const override final;
  bool compare(const String&, const bool& i = false) const;
 private:
  std::string str_;
  std::wstring wstr_;
  std::u8string u8_;
 };



}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 28 Jun 2023 10:39:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__82970C26_58E9_4899_87EC_F2374D6547D8__