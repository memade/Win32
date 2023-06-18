#if !defined(__F3F67582_23F2_4B69_8D09_EEAB047AE930__)
#define __F3F67582_23F2_4B69_8D09_EEAB047AE930__

namespace local {

 class Headers final {
 public:
  Headers();
  ~Headers();
 public:
  void operator<<(const std::string&);
  void operator>>(std::list<std::string>&) const;
  void operator>>(std::string&);
  void push(const std::string&, const std::string&);
  std::string search(const std::string&) const;
 private:
  std::map<std::string, std::string> m_Headers;
  std::string m_Source;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };



}///namespace local 



/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 07 Jun 2023 05:04:07 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__F3F67582_23F2_4B69_8D09_EEAB047AE930__

