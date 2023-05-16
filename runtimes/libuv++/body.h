#if !defined(__8906D021_E8C9_4C41_88A7_790E12D84469__)
#define __8906D021_E8C9_4C41_88A7_790E12D84469__

namespace local {

 class Body final {
 public:
  Body(const char*);
  Body(const std::string&);
  Body();
  ~Body();
 public:
  void clear();
  bool empty() const;
  bool operator<<(const std::string&);
  void operator>>(std::string&) const;
 private:
  cjson::cJSON* m_json_root = nullptr;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 12 May 2023 05:16:14 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__8906D021_E8C9_4C41_88A7_790E12D84469__