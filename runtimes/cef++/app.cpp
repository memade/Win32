﻿#include "stdafx.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"


namespace local {

 namespace {
  class SimpleWindowDelegate : public CefWindowDelegate {
  public:
   explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
    : browser_view_(browser_view) {}
   void OnWindowCreated(CefRefPtr<CefWindow> window) override {
    window->AddChildView(browser_view_);
    window->Show();
    browser_view_->RequestFocus();
   }

   void OnWindowDestroyed(CefRefPtr<CefWindow> window) override {
    browser_view_ = nullptr;
   }

   bool CanClose(CefRefPtr<CefWindow> window) override {
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
     return browser->GetHost()->TryCloseBrowser();
    return true;
   }

   CefSize GetPreferredSize(CefRefPtr<CefView> view) override {
    return CefSize(800, 600);
   }
  private:
   CefRefPtr<CefBrowserView> browser_view_;
   IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
   DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
  };

  class SimpleBrowserViewDelegate : public CefBrowserViewDelegate {
  public:
   SimpleBrowserViewDelegate() {}

   bool OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view,
    CefRefPtr<CefBrowserView> popup_browser_view,
    bool is_devtools) override {
    CefWindow::CreateTopLevelWindow(
     new SimpleWindowDelegate(popup_browser_view));
    return true;
   }

  private:
   IMPLEMENT_REFCOUNTING(SimpleBrowserViewDelegate);
   DISALLOW_COPY_AND_ASSIGN(SimpleBrowserViewDelegate);
  };

 }  // namespace

 ICefApp::ICefApp() {}

 void ICefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
  const bool use_views = command_line->HasSwitch("use-views");
  CefRefPtr<ICefClient> handler(new ICefClient(use_views));
  CefBrowserSettings browser_settings;
  std::string url;
  url = command_line->GetSwitchValue("url");
  if (url.empty())
   url = "https://cn.bing.com";

  if (use_views) {
   CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
    handler, url, browser_settings, nullptr, nullptr,
    new SimpleBrowserViewDelegate());

   CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
  }
  else {
   CefWindowInfo window_info;
#if defined(OS_WIN)
   window_info.SetAsPopup(nullptr, "cefsimple");
#endif
   CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
    nullptr, nullptr);
  }
 }
 CefRefPtr<CefClient> ICefApp::GetDefaultClient() {
  return ICefClient::GetInstance();
 }


}///namespace local
