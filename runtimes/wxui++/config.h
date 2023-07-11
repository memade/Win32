#if !defined(__202F018A_9AC9_4B9B_A851_5A7377151D84__)
#define __202F018A_9AC9_4B9B_A851_5A7377151D84__

namespace local {

 class Config final : public IConfig {
 public:
  Config();
  virtual ~Config();
  void Release() const;
 private:
  void Init();
  void UnInit();
 protected:
  //FrameType FrameTypeGet() const override final;
  //void FrameTypeSet(const FrameType&) override final;
  void MainWindowInitialShow(const bool&) override final;
  void RequestUrl(const char*) override final;
  void EnableChildWindowExclusiveMode(const bool&) override final;
  bool EnableChildWindowExclusiveMode() const override final;
 public:
  bool MainWindowInitialShow() const;
  const std::string& RequestUrl() const;
 private:
  bool m_bMainWindowInitialShow = false;
  bool m_EnableChildWindowExclusiveMode = false;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
  std::string m_RequesUrl;
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:48 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__202F018A_9AC9_4B9B_A851_5A7377151D84__