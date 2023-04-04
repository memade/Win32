#include "stdafx.h"
#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
namespace cef {
 namespace {
  ICefClient* g_instance = nullptr;
  std::string GetDataURI(const std::string& data, const std::string& mime_type) {
   return "data:" + mime_type + ";base64," +
    CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
    .ToString();
  }

 } // namespace

 ICefClient::ICefClient(bool use_views)
  : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
 }

 ICefClient::~ICefClient() {
  g_instance = nullptr;
 }
 ICefClient* ICefClient::GetInstance() {
  return g_instance;
 }

 void ICefClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
   CefRefPtr<CefBrowserView> browser_view =
    CefBrowserView::GetForBrowser(browser);
   if (browser_view) {
    CefRefPtr<CefWindow> window = browser_view->GetWindow();
    if (window)
     window->SetTitle(title);
   }
  }
  else if (!IsChromeRuntimeEnabled()) {
   PlatformTitleChange(browser, title);
  }
 }

 void ICefClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  browser_list_.push_back(browser);
 }

 bool ICefClient::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  if (browser_list_.size() == 1) {
   is_closing_ = true;
  }
  return false;
 }

 void ICefClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
   if ((*bit)->IsSame(browser)) {
    browser_list_.erase(bit);
    break;
   }
  }

  if (browser_list_.empty()) {
   CefQuitMessageLoop();
  }
 }

 void ICefClient::OnLoadError(CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  ErrorCode errorCode,
  const CefString& errorText,
  const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();
  if (IsChromeRuntimeEnabled())
   return;
  if (errorCode == ERR_ABORTED)
   return;
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
   "<h2>Failed to load URL "
   << std::string(failedUrl) << " with error " << std::string(errorText)
   << " (" << errorCode << ").</h2></body></html>";

  frame->LoadURL(GetDataURI(ss.str(), "text/html"));
 }

 void ICefClient::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
   CefPostTask(TID_UI, base::BindOnce(&ICefClient::CloseAllBrowsers, this,
    force_close));
   return;
  }

  if (browser_list_.empty())
   return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
   (*it)->GetHost()->CloseBrowser(force_close);
 }

 bool ICefClient::IsChromeRuntimeEnabled() {
  static int value = -1;
  if (value == -1) {
   CefRefPtr<CefCommandLine> command_line =
    CefCommandLine::GetGlobalCommandLine();
   value = command_line->HasSwitch("enable-chrome-runtime") ? 1 : 0;
  }
  return value == 1;
 }



}///namespace cef