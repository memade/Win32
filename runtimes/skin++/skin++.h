#if !defined(__C40A8379_718F_4F38_85BC_078872DAECC4__)
#define __C40A8379_718F_4F38_85BC_078872DAECC4__

namespace local {

 class Skin final : public ISkin {
 public:
  Skin();
  virtual ~Skin();
 private:
  void Init();
  void UnInit();
 public:
  void Release() const override final;
  bool Perform() override final;
  void Render() override final;
  void Layout() override final;
  void SetUIModule(ISkinUI*) override final;
  bool SkinConfigure(const char*) override final;
  bool Ready() const override final;
 private:
  std::atomic_bool m_IsOpen = false;
  //!@ UI initialization rendering completed | ready
  std::atomic_bool m_Ready = false;
  ISkinUI* m_pUI = nullptr;
  std::string m_SkinDirectory;
  std::string m_SkinFileName;
  std::string m_SkinFileBuffer;
  std::vector<Control*> m_Controls;
  IResources* m_pResources = nullptr;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };



 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 00:26:33 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C40A8379_718F_4F38_85BC_078872DAECC4__
