#if !defined(__0B46F7CA_5FB9_466A_B462_BD0C1711817E__)
#define __0B46F7CA_5FB9_466A_B462_BD0C1711817E__

namespace local {

 class Cache final : public ICache {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Cache();
  Cache(const std::string&);
  Cache(const std::vector<char>&);
  virtual ~Cache();
  void operator<<(const std::string&);
  void operator<<(const std::vector<char>&);
  void operator=(const Cache&);
  void Release() const override final;
 protected:
  const char* Buffer() const override final;
  const size_t& Size() const override final;
 private:
  char* buffer_ = nullptr;
  size_t size_ = 0;
 };

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 02 Jul 2023 12:44:25 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0B46F7CA_5FB9_466A_B462_BD0C1711817E__