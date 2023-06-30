#if !defined(__202F018A_9AC9_4B9B_A851_5A7377151D84__)
#define __202F018A_9AC9_4B9B_A851_5A7377151D84__

namespace local {

 class Config final : public IConfig {
 public:
  Config();
  virtual ~Config();
  void Release() const override final;
 private:
  void Init();
  void UnInit();
 protected:
  void MainWindowInitialShow(const bool&) override final;
  bool MainWindowInitialShow() const override final;
  void EnableDpiAwareness(const bool&) override final;
  bool EnableDpiAwareness() const override final;
 private:
  bool m_bMainWindowInitialShow = false;
  bool m_bEnableDpiAwareness = false;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:48 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__202F018A_9AC9_4B9B_A851_5A7377151D84__