#if !defined(__88AB3DFC_5633_453A_9CC7_D1336BBF9CB7__)
#define __88AB3DFC_5633_453A_9CC7_D1336BBF9CB7__


namespace local {

 class Parser final : public cmd::IParser {
 public:
  Parser();
  virtual ~Parser();
 public:
  void Release() const override final;
 protected:
  void SetCmdLine(const char*) override final;
  bool Ready() const override final;
  bool Perform() override final;
  void SetSwitchChars(const char*) override final;
  void SetLogo(const char*) override final;
  void AddSwitch(const char*) override final;
  void AddLongSwitch(const char*) override final;
  void AddOption(const char* option, const char* value) override final;
  void AddLongOption(const char* name, const char* value) override final;
  void AddParam(const char*) override final;
  void GetParam(const size_t&, const tfOnAllcoStringCb&) const override final;
  bool Found(const char* name) const override final;
  bool FoundSwitch(const char* name) const override final;
  bool Found(const char* name, const tfOnAllcoStringCb&) const override final;
  bool Found(const char* name, long& value) const override final;
  bool Found(const char* name, double& value) const override final;
 private:
  void Init();
  void UnInit();
  std::atomic_bool m_Ready = false;
  wxString m_CommandLineText;
  wxCmdLineParser* m_pWxParser = nullptr;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 30 May 2023 13:15:43 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__88AB3DFC_5633_453A_9CC7_D1336BBF9CB7__
