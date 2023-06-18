#if !defined(__B2F8CFA3_C4D3_4FB4_A630_582A7AB93361__)
#define __B2F8CFA3_C4D3_4FB4_A630_582A7AB93361__

namespace local {
 class Cache final {
 public:
  Cache(const std::string&);
  virtual ~Cache();
  void Release() const;
 public:
  size_t Size() const;
  std::string Get() const;
  bool Ready() const;
  bool operator<<(const std::string&);
  bool operator>>(std::string&);
 private:
  void Init();
  void UnInit();
  std::ofstream* m_pOfstream = nullptr;
  std::atomic_bool m_Ready = false;
  std::atomic_size_t m_CurrentSize = 0;
  const std::string m_cache_filepathname;
  const std::ios_base::openmode m_openmode = std::ios::binary | std::ios::out | std::ios::app;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 07 Jun 2023 02:41:56 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B2F8CFA3_C4D3_4FB4_A630_582A7AB93361__

