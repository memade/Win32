#if !defined(__B03BF7AD_2476_415D_8479_2F5FCE1A855B__)
#define __B03BF7AD_2476_415D_8479_2F5FCE1A855B__

#ifdef IsMinimized
#undef IsMinimized
#endif


#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/wrapper/cef_helpers.h>
#include <include/cef_command_line.h>

#include <include/cef_sandbox_win.h>

namespace cef {

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

}///namespace cef


#pragma comment(lib,"cef++.lib")
#pragma comment(lib,"libcef.lib")
#pragma comment(lib,"libcef_dll_wrapper.lib")
#if !_DEBUG
#pragma comment(lib,"cef_sandbox.lib")
#if CEF_USE_SANDBOX
#endif
#endif
/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 03 Apr 2023 06:37:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B03BF7AD_2476_415D_8479_2F5FCE1A855B__