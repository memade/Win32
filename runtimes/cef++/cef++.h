#if !defined(__DD307DDA_6334_410A_8880_368A422249DB__)
#define __DD307DDA_6334_410A_8880_368A422249DB__

namespace local {

 class ICefApp : public CefApp, public CefBrowserProcessHandler {
 public:
  ICefApp();
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
   return this;
  }
  void OnContextInitialized() override;
  CefRefPtr<CefClient> GetDefaultClient() override;
 private:
  IMPLEMENT_REFCOUNTING(ICefApp);
 };

 class ICefClient : public CefClient,
  public CefDisplayHandler,
  public CefLifeSpanHandler,
  public CefLoadHandler {
 public:
  explicit ICefClient(bool use_views);
  ~ICefClient();
  static ICefClient* GetInstance();
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
   return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
   return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
   const CefString& title) override;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   ErrorCode errorCode,
   const CefString& errorText,
   const CefString& failedUrl) override;
  void CloseAllBrowsers(bool force_close);
  bool IsClosing() const { return is_closing_; }
  static bool IsChromeRuntimeEnabled();
 private:
  void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
   const CefString& title);
  const bool use_views_;
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;
  bool is_closing_;
  IMPLEMENT_REFCOUNTING(ICefClient);
 };


 class Cef final : public ICef {
 public:
  Cef();
  virtual ~Cef();
 private:
  void Init();
  void UnInit();
 protected:
  IConfig* ConfigGet() const override final;
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  void Show(const bool&) const override final;
  HWND MainWnd() const override final;
  HANDLE MainProcess() const override final;
  HWND CreateFrameChild(const bool&) override final;
  HWND CreateFrameChildHost(const bool&) override final { return nullptr; }
  void RegisterOnDestroyCb(const tfOnDestroyCb&) override final;
 private:
  static unsigned int MainProcess(void*);
  std::atomic_bool m_IsOpen = false;
  Config* m_pConfig = nullptr;
  HANDLE m_hMain = nullptr;
 };


 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 02 Jun 2023 11:43:05 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DD307DDA_6334_410A_8880_368A422249DB__
