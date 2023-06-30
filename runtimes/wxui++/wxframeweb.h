#if !defined(__94A7399D_DE36_4FC7_9187_B821AD4D260B__)
#define __94A7399D_DE36_4FC7_9187_B821AD4D260B__

namespace local {
#if 0
 class Wxui;
 class FrameWeb
  : public wxFrame
  , public IFrame {
 public:
  FrameWeb(const Wxui* host);
  virtual ~FrameWeb();
 public:
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  IChildFrame* CreateChildFrame() override final { return nullptr; }
  IChildFrame* CreateChildFrameHost(const bool&) override final { return nullptr; }
 protected:
  void RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb&) override final { return; }
  void RegisterOnFrameCreateChildHostCb(const tfOnFrameCreateChildHostCb&) override final { return; }
 private:
  std::map<std::string,wxWebView*> m_Browsers;
  void OnNavigationRequest(wxWebViewEvent& evt);
  void OnNavigationComplete(wxWebViewEvent& evt);
  void OnDocumentLoaded(wxWebViewEvent& evt);
  void OnNewWindow(wxWebViewEvent& evt);
  void OnTitleChanged(wxWebViewEvent& evt);
  void OnFullScreenChanged(wxWebViewEvent& evt);
  void OnScriptMessage(wxWebViewEvent& evt);
  void OnError(wxWebViewEvent& evt);
  void OnScriptResult(wxWebViewEvent& evt);
 private:
  const Wxui* m_pWxui;
  wxWebView* m_pBrowser = nullptr;
 };

#if 0
 //We map menu items to their history items
 WX_DECLARE_HASH_MAP(int, wxSharedPtr<wxWebViewHistoryItem>,
  wxIntegerHash, wxIntegerEqual, wxMenuHistoryMap);

 class FrameWeb
  : public wxFrame
  , public IFrame {
 public:
  FrameWeb(const wxString& url);
  virtual ~FrameWeb();

  void UpdateState();
  void OnIdle(wxIdleEvent& evt);
  void OnUrl(wxCommandEvent& evt);
  void OnBack(wxCommandEvent& evt);
  void OnForward(wxCommandEvent& evt);
  void OnStop(wxCommandEvent& evt);
  void OnReload(wxCommandEvent& evt);
  void OnClearHistory(wxCommandEvent& evt);
  void OnEnableHistory(wxCommandEvent& evt);
  void OnNavigationRequest(wxWebViewEvent& evt);
  void OnNavigationComplete(wxWebViewEvent& evt);
  void OnDocumentLoaded(wxWebViewEvent& evt);
  void OnNewWindow(wxWebViewEvent& evt);
  void OnTitleChanged(wxWebViewEvent& evt);
  void OnFullScreenChanged(wxWebViewEvent& evt);
  void OnScriptMessage(wxWebViewEvent& evt);
  void OnError(wxWebViewEvent& evt);
  void OnScriptResult(wxWebViewEvent& evt);
  void OnSetPage(wxCommandEvent& evt);
  void OnViewSourceRequest(wxCommandEvent& evt);
  void OnViewTextRequest(wxCommandEvent& evt);
  void OnToolsClicked(wxCommandEvent& evt);
  void OnSetZoom(wxCommandEvent& evt);
  void OnPrint(wxCommandEvent& evt);
  void OnCut(wxCommandEvent& evt);
  void OnCopy(wxCommandEvent& evt);
  void OnPaste(wxCommandEvent& evt);
  void OnUndo(wxCommandEvent& evt);
  void OnRedo(wxCommandEvent& evt);
  void OnMode(wxCommandEvent& evt);
  void OnZoomLayout(wxCommandEvent& evt);
  void OnZoomCustom(wxCommandEvent& evt);
  void OnHistory(wxCommandEvent& evt);
  void OnScrollLineUp(wxCommandEvent&) { m_browser->LineUp(); }
  void OnScrollLineDown(wxCommandEvent&) { m_browser->LineDown(); }
  void OnScrollPageUp(wxCommandEvent&) { m_browser->PageUp(); }
  void OnScrollPageDown(wxCommandEvent&) { m_browser->PageDown(); }
  void RunScript(const wxString& javascript);
  void OnRunScriptString(wxCommandEvent& evt);
  void OnRunScriptInteger(wxCommandEvent& evt);
  void OnRunScriptDouble(wxCommandEvent& evt);
  void OnRunScriptBool(wxCommandEvent& evt);
  void OnRunScriptObject(wxCommandEvent& evt);
  void OnRunScriptArray(wxCommandEvent& evt);
  void OnRunScriptDOM(wxCommandEvent& evt);
  void OnRunScriptUndefined(wxCommandEvent& evt);
  void OnRunScriptNull(wxCommandEvent& evt);
  void OnRunScriptDate(wxCommandEvent& evt);
#if wxUSE_WEBVIEW_IE
  void OnRunScriptObjectWithEmulationLevel(wxCommandEvent& evt);
  void OnRunScriptDateWithEmulationLevel(wxCommandEvent& evt);
  void OnRunScriptArrayWithEmulationLevel(wxCommandEvent& evt);
#endif
  void OnRunScriptMessage(wxCommandEvent& evt);
  void OnRunScriptAsync(wxCommandEvent& evt);
  void OnRunScriptCustom(wxCommandEvent& evt);
  void OnAddUserScript(wxCommandEvent& evt);
  void OnSetCustomUserAgent(wxCommandEvent& evt);
  void OnClearSelection(wxCommandEvent& evt);
  void OnDeleteSelection(wxCommandEvent& evt);
  void OnSelectAll(wxCommandEvent& evt);
  void OnLoadScheme(wxCommandEvent& evt);
  void OnUseMemoryFS(wxCommandEvent& evt);
  void OnFind(wxCommandEvent& evt);
  void OnFindDone(wxCommandEvent& evt);
  void OnFindText(wxCommandEvent& evt);
  void OnFindOptions(wxCommandEvent& evt);
  void OnEnableContextMenu(wxCommandEvent& evt);
  void OnEnableDevTools(wxCommandEvent& evt);
 public:
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  IChildFrame* CreateChildFrame() override final { return nullptr; }
 protected:
  void RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb&) override final { return; }
 private:
  wxTextCtrl* m_url;
  wxWebView* m_browser;

  wxToolBar* m_toolbar;
  wxToolBarToolBase* m_toolbar_back;
  wxToolBarToolBase* m_toolbar_forward;
  wxToolBarToolBase* m_toolbar_stop;
  wxToolBarToolBase* m_toolbar_reload;
  wxToolBarToolBase* m_toolbar_tools;

  wxToolBarToolBase* m_find_toolbar_done;
  wxToolBarToolBase* m_find_toolbar_next;
  wxToolBarToolBase* m_find_toolbar_previous;
  wxToolBarToolBase* m_find_toolbar_options;
  wxMenuItem* m_find_toolbar_wrap;
  wxMenuItem* m_find_toolbar_highlight;
  wxMenuItem* m_find_toolbar_matchcase;
  wxMenuItem* m_find_toolbar_wholeword;

  wxMenu* m_tools_menu;
  wxMenu* m_tools_history_menu;
  wxMenuItem* m_tools_layout;
  wxMenuItem* m_tools_tiny;
  wxMenuItem* m_tools_small;
  wxMenuItem* m_tools_medium;
  wxMenuItem* m_tools_large;
  wxMenuItem* m_tools_largest;
  wxMenuItem* m_tools_custom;
  wxMenuItem* m_tools_handle_navigation;
  wxMenuItem* m_tools_handle_new_window;
  wxMenuItem* m_tools_enable_history;
  wxMenuItem* m_edit_cut;
  wxMenuItem* m_edit_copy;
  wxMenuItem* m_edit_paste;
  wxMenuItem* m_edit_undo;
  wxMenuItem* m_edit_redo;
  wxMenuItem* m_edit_mode;
  wxMenuItem* m_scroll_line_up;
  wxMenuItem* m_scroll_line_down;
  wxMenuItem* m_scroll_page_up;
  wxMenuItem* m_scroll_page_down;
  wxMenuItem* m_script_string;
  wxMenuItem* m_script_integer;
  wxMenuItem* m_script_double;
  wxMenuItem* m_script_bool;
  wxMenuItem* m_script_object;
  wxMenuItem* m_script_array;
  wxMenuItem* m_script_dom;
  wxMenuItem* m_script_undefined;
  wxMenuItem* m_script_null;
  wxMenuItem* m_script_date;
#if wxUSE_WEBVIEW_IE
  wxMenuItem* m_script_object_el;
  wxMenuItem* m_script_date_el;
  wxMenuItem* m_script_array_el;
#endif
  wxMenuItem* m_script_message;
  wxMenuItem* m_script_custom;
  wxMenuItem* m_script_async;
  wxMenuItem* m_selection_clear;
  wxMenuItem* m_selection_delete;
  wxMenuItem* m_find;
  wxMenuItem* m_context_menu;
  wxMenuItem* m_dev_tools;

  wxInfoBar* m_info;
  wxStaticText* m_info_text;
  wxTextCtrl* m_find_ctrl;
  wxToolBar* m_find_toolbar;

  wxMenuHistoryMap m_histMenuItems;
  wxString m_findText;
  int m_findFlags, m_findCount;
  long m_zoomFactor;

  // Last executed JavaScript snippet, for convenience.
  wxString m_javascript;
 };
#endif

#endif
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 28 May 2023 09:41:09 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__94A7399D_DE36_4FC7_9187_B821AD4D260B__
