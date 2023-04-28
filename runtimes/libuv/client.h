#if !defined(__3C5924E0_D62E_4662_A8ED_BB4C6921F96C__)
#define __3C5924E0_D62E_4662_A8ED_BB4C6921F96C__

namespace local {

 class Client final : public IClient {
 public:
  Client();
  virtual ~Client();
 public:
  void Release() const override final;
  bool Start() override final;
  void Stop() override final;
  IConfig* ConfigGet() const override final;
  ConnectionStatus ConnStatus() const override final;
 private:
  Session* m_pSession = nullptr;
  Config* m_pConfig = nullptr;
  std::atomic_bool m_IsOpen = false;
  void Process();
  std::vector<std::thread> m_Threads;
  shared::Log* m_pLog = nullptr;
 };

 extern Client* __gpClient;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:21:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__3C5924E0_D62E_4662_A8ED_BB4C6921F96C__

