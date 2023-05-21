#if !defined(__2BFBE9A5_EDD9_447B_A673_9BACBBD7CBB8__)
#define __2BFBE9A5_EDD9_447B_A673_9BACBBD7CBB8__

namespace local {

 class Client final : public IClient {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Client();
  ~Client();
 public:
  Config* ConfigGet() const override final;
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  ServerStatus Status() const override final;
  bool Close() const; 
 private:
  void Init();
  void UnInit();
  std::atomic_bool m_IsOpen = false;
  Config* m_pConfig = nullptr;
  Session* m_pSession = nullptr;
  ServerStatus m_ServerStatus = ServerStatus::UNKNOWN;
 };

 extern Client* __gpClient;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:23:40 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__2BFBE9A5_EDD9_447B_A673_9BACBBD7CBB8__