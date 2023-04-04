#include "stdafx.h"
#include <include/cef_browser.h>
namespace cef {

 void ICefClient::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
  const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  if (hwnd)
   SetWindowText(hwnd, std::wstring(title).c_str());
 }

}///namespace cef