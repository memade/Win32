#if !defined(__B986614D_5646_47EA_BF65_00A1C33CC211__)
#define __B986614D_5646_47EA_BF65_00A1C33CC211__

namespace local {

 class Cmd final : public cmd::ICmd {
 public:
  Cmd();
  virtual ~Cmd();
 protected:
  void Release() const override final;
  IParser* ParserCreate() override final;
  bool ParserDestory(IParser*&) override final;
 private:
  void Init();
  void UnInit();
  std::atomic_bool m_Ready = false;
  std::set<Parser*> m_Parsers;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:45:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B986614D_5646_47EA_BF65_00A1C33CC211__