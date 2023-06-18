#if !defined(__F0C1DDF7_7CAD_446A_A9D2_69ECBE6469D4__)
#define __F0C1DDF7_7CAD_446A_A9D2_69ECBE6469D4__

namespace local {

 class Curl final : public ICurl {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Curl();
  virtual ~Curl();
 protected:
  void Release() const override final;
  bool Start() override final;
  void Stop() override final;
  IRequest* CreateRequest(const char* url) override final;
  bool DestroyRequest(const IRequest*&) override final;
  void RegisterRequestDoneCb(const tfRequestDoneCb&) override final;
  void RegisterRequestBeginCb(const tfRequestDoneCb&) override final;
  void RegisterRequestRemoveCb(const tfRequestRemoveCb&) override final;
 public:
  IConfig* ConfigGet() const override final;
  void OnRequestDone(const IRequest*) const;
  void OnRequestBegin(const IRequest*) const;
  void OnRequestRemove(const IRequest*) const;
 private:
  curlpp::Cleanup cleaner;
  void Init();
  void UnInit();
  void Process();
  std::vector<std::thread> m_Threads;
  Config* m_pConfig = nullptr;
  std::atomic_bool m_IsOpen = false;
  std::map<TypeIdentify, Request*, std::less<TypeIdentify>> m_Requests;
  tfRequestDoneCb m_RequestDoneCb = nullptr;
  tfRequestBeginCb m_RequestBeginCb = nullptr;
  tfRequestRemoveCb m_RequestRemoveCb = nullptr;
 public:
  static RequestType GetRealRequestType(const TypeRequestType&);
  static bool IsPreparationRequestType(const TypeRequestType&);
 };


}///namespace local 



/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 05 Jun 2023 13:39:03 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__F0C1DDF7_7CAD_446A_A9D2_69ECBE6469D4__