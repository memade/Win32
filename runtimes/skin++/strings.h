#if !defined(__82970C26_58E9_4899_87EC_F2374D6547D8__)
#define __82970C26_58E9_4899_87EC_F2374D6547D8__

namespace local {

 class Strings final : public IStrings {
 public:
  Strings();
  Strings(const std::string&);
  Strings(const std::wstring&);
  Strings(const std::u8string&);
  virtual ~Strings();
  void operator=(const Strings&);
  void operator<<(const std::string&);
  void operator<<(const std::wstring&);
  void operator<<(const std::u8string&);
  const std::string& toString() const;
  const std::wstring& toStringW() const;
  const std::u8string& toStringU8() const;
 public:
  const char* str() const override final;
  const wchar_t* wstr() const override final;
  const char8_t* u8() const override final;
  bool empty() const override final;
  bool compare(const wchar_t*, const bool& i = false) const override final;
  bool compare(const char*, const bool& i = false) const override final;
  bool compare(const IStrings*, const bool& i = false) const override final;
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