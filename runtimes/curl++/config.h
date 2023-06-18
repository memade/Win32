#if !defined(__83B7B86B_6223_4009_88E0_1C83003EF5B5__)
#define __83B7B86B_6223_4009_88E0_1C83003EF5B5__

namespace local {
 class Config final : public IConfig {
 public:
  Config();
  virtual ~Config();
 private:
  void Init();
  void UnInit();
 public:
  bool EnableSSLSupport() const override final;
  bool EnableDefaultPrerequestedHeader() const;
  std::uint64_t StatusUpdateInterval() const;
  std::uint64_t ProcessExecuteInterval() const;
  std::uint8_t MaximumNumberOfTasks() const;
  const std::string& DownCacheDirectory() const;
 protected:
  void EnableSSLSupport(const bool&) override final;
  void EnableDefaultPrerequestedHeader(const bool& enable) override final;
  void StatusUpdateInterval(const std::uint64_t&) override final;
  void ProcessExecuteInterval(const std::uint64_t&) override final;
  void MaximumNumberOfTasks(const std::uint8_t&) override final;
  void DownCacheDirectory(const char*) override final;
 private:
  bool m_EnableSSLSupport = false;
  bool m_EnableDefaultPrerequestedHeader = false;
  std::uint64_t m_StatusUpdateInterval;
  std::uint64_t  m_ProcessExecuteInterval;
  std::uint8_t m_MaximumNumberOfTasks;
  std::string m_DownCacheDirectory;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };
}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 15:03:30 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__83B7B86B_6223_4009_88E0_1C83003EF5B5__