#include "stdafx.h"

namespace local {
#if 0
 FrameWeb::FrameWeb(const Wxui* host)
  : wxFrame(NULL, wxID_ANY, L"wxWebView®")
  , m_pWxui(host) {

#if 0
  auto requrl = reinterpret_cast<Config*>(m_pWxui->ConfigGet())->RequestUrl();

  // Create the webview
  m_pBrowser = wxWebView::New();
  m_pBrowser->Create(this, wxID_ANY, requrl, wxDefaultPosition, wxDefaultSize);
#endif
  // Connect the webview events
  Bind(wxEVT_WEBVIEW_NAVIGATING, &FrameWeb::OnNavigationRequest, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_NAVIGATED, &FrameWeb::OnNavigationComplete, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_LOADED, &FrameWeb::OnDocumentLoaded, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_ERROR, &FrameWeb::OnError, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_NEWWINDOW, &FrameWeb::OnNewWindow, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_TITLE_CHANGED, &FrameWeb::OnTitleChanged, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_FULLSCREEN_CHANGED, &FrameWeb::OnFullScreenChanged, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FrameWeb::OnScriptMessage, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_SCRIPT_RESULT, &FrameWeb::OnScriptResult, this, m_pBrowser->GetId());
 }

 FrameWeb::~FrameWeb() {
  Unbind(wxEVT_WEBVIEW_NAVIGATING, &FrameWeb::OnNavigationRequest, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_NAVIGATED, &FrameWeb::OnNavigationComplete, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_LOADED, &FrameWeb::OnDocumentLoaded, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_ERROR, &FrameWeb::OnError, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_NEWWINDOW, &FrameWeb::OnNewWindow, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_TITLE_CHANGED, &FrameWeb::OnTitleChanged, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_FULLSCREEN_CHANGED, &FrameWeb::OnFullScreenChanged, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FrameWeb::OnScriptMessage, this, m_pBrowser->GetId());
  Unbind(wxEVT_WEBVIEW_SCRIPT_RESULT, &FrameWeb::OnScriptResult, this, m_pBrowser->GetId());
 }
 void FrameWeb::Show(const bool& show) {
  wxFrame::Show(show);
 }

 HWND FrameWeb::Hwnd() const {
  return wxFrame::GetHWND();
 }

 void FrameWeb::OnNavigationRequest(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnNavigationComplete(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnDocumentLoaded(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnNewWindow(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnTitleChanged(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnFullScreenChanged(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnScriptMessage(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnError(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
 void FrameWeb::OnScriptResult(wxWebViewEvent& evt) {
  wxString url = evt.GetURL();

  auto xx = 0;
 }
















#if 0
 void FrameWeb::Show(const bool& show) {
  wxFrame::Show(show);
 }

 HWND FrameWeb::Hwnd() const {
  return wxFrame::GetHWND();
 }


 class SourceViewDialog : public wxDialog
 {
 public:
  SourceViewDialog(wxWindow* parent, wxString source);
 };

 FrameWeb::FrameWeb(const wxString& url) :
  wxFrame(NULL, wxID_ANY, "wxWebView Sample")
 {
  // set the frame icon
  SetIcon(wxICON(sample));
  SetTitle("wxWebView Sample");
  EnableFullScreenView(); // Enable native fullscreen API on macOS

  wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

  // Create the toolbar
  m_toolbar = CreateToolBar(wxTB_TEXT);

  m_toolbar_back = m_toolbar->AddTool(wxID_ANY, _("Back"), wxArtProvider::GetBitmapBundle(wxART_GO_BACK, wxART_TOOLBAR));
  m_toolbar_forward = m_toolbar->AddTool(wxID_ANY, _("Forward"), wxArtProvider::GetBitmapBundle(wxART_GO_FORWARD, wxART_TOOLBAR));
  m_toolbar_stop = m_toolbar->AddTool(wxID_ANY, _("Stop"), wxArtProvider::GetBitmapBundle(wxART_STOP, wxART_TOOLBAR));
  m_toolbar_reload = m_toolbar->AddTool(wxID_ANY, _("Reload"), wxArtProvider::GetBitmapBundle(wxART_REFRESH, wxART_TOOLBAR));
  m_url = new wxTextCtrl(m_toolbar, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(400, -1)), wxTE_PROCESS_ENTER);
  m_toolbar->AddControl(m_url, _("URL"));
  m_toolbar_tools = m_toolbar->AddTool(wxID_ANY, _("Menu"), wxArtProvider::GetBitmapBundle(wxART_WX_LOGO, wxART_TOOLBAR));

  m_toolbar->Realize();

  // Set find values.
  m_findFlags = wxWEBVIEW_FIND_DEFAULT;
  m_findCount = 0;

  // Create panel for find toolbar.
  wxPanel* panel = new wxPanel(this);
  topsizer->Add(panel, wxSizerFlags().Expand());

  // Create sizer for panel.
  wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(panel_sizer);

  // Create the find toolbar.
  m_find_toolbar = new wxToolBar(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxTB_HORZ_LAYOUT);
  m_find_toolbar->Hide();
  panel_sizer->Add(m_find_toolbar, wxSizerFlags().Expand());

  // Create find control.
  m_find_ctrl = new wxTextCtrl(m_find_toolbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1), wxTE_PROCESS_ENTER);


  //Find options menu
  wxMenu* findmenu = new wxMenu;
  m_find_toolbar_wrap = findmenu->AppendCheckItem(wxID_ANY, "Wrap");
  m_find_toolbar_matchcase = findmenu->AppendCheckItem(wxID_ANY, "Match Case");
  m_find_toolbar_wholeword = findmenu->AppendCheckItem(wxID_ANY, "Entire Word");
  m_find_toolbar_highlight = findmenu->AppendCheckItem(wxID_ANY, "Highlight");
  // Add find toolbar tools.
  m_find_toolbar->SetToolSeparation(7);
  m_find_toolbar_done = m_find_toolbar->AddTool(wxID_ANY, "Close", wxArtProvider::GetBitmap(wxART_CROSS_MARK));
  m_find_toolbar->AddSeparator();
  m_find_toolbar->AddControl(m_find_ctrl, "Find");
  m_find_toolbar->AddSeparator();
  m_find_toolbar_next = m_find_toolbar->AddTool(wxID_ANY, "Next", wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_TOOLBAR, wxSize(16, 16)));
  m_find_toolbar_previous = m_find_toolbar->AddTool(wxID_ANY, "Previous", wxArtProvider::GetBitmap(wxART_GO_UP, wxART_TOOLBAR, wxSize(16, 16)));
  m_find_toolbar->AddSeparator();
  m_find_toolbar_options = m_find_toolbar->AddTool(wxID_ANY, "Options", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR, wxSize(16, 16)), "", wxITEM_DROPDOWN);
  m_find_toolbar_options->SetDropdownMenu(findmenu);
  m_find_toolbar->Realize();

  // Create the info panel
  m_info = new wxInfoBar(this);
  topsizer->Add(m_info, wxSizerFlags().Expand());

  // Create a log window
  new wxLogWindow(this, _("Logging"), true, false);

#if wxUSE_WEBVIEW_EDGE
  // Check if a fixed version of edge is present in
  // $executable_path/edge_fixed and use it
  wxFileName edgeFixedDir(wxStandardPaths::Get().GetExecutablePath());
  edgeFixedDir.SetFullName("");
  edgeFixedDir.AppendDir("edge_fixed");
  if (edgeFixedDir.DirExists())
  {
   wxWebViewEdge::MSWSetBrowserExecutableDir(edgeFixedDir.GetFullPath());
   wxLogMessage("Using fixed edge version");
  }
#endif
  // Create the webview
  m_pBrowser = wxWebView::New();
#ifdef __WXMAC__
  // With WKWebView handlers need to be registered before creation
  m_pBrowser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewArchiveHandler("wxfs")));
  m_pBrowser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));
#endif
  m_pBrowser->Create(this, wxID_ANY, url, wxDefaultPosition, wxDefaultSize);
  topsizer->Add(m_pBrowser, wxSizerFlags().Expand().Proportion(1));

  // Log backend information
  wxLogMessage("Backend: %s Version: %s", m_pBrowser->GetClassInfo()->GetClassName(),
   wxWebView::GetBackendVersionInfo().ToString());
  wxLogMessage("User Agent: %s", m_pBrowser->GetUserAgent());

#ifndef __WXMAC__
  //We register the wxfs:// protocol for testing purposes
  m_pBrowser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewArchiveHandler("wxfs")));
  //And the memory: file system
  m_pBrowser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));
#endif
  if (!m_pBrowser->AddScriptMessageHandler("wx"))
   wxLogError("Could not add script message handler");

  SetSizer(topsizer);

  //Set a more sensible size for web browsing
  SetSize(FromDIP(wxSize(800, 600)));

  // Create the Tools menu
  m_tools_menu = new wxMenu();
  wxMenuItem* print = m_tools_menu->Append(wxID_ANY, _("Print"));
  wxMenuItem* setPage = m_tools_menu->Append(wxID_ANY, _("Set page text"));
  wxMenuItem* viewSource = m_tools_menu->Append(wxID_ANY, _("View Source"));
  wxMenuItem* viewText = m_tools_menu->Append(wxID_ANY, _("View Text"));
  m_tools_menu->AppendSeparator();
  m_tools_layout = m_tools_menu->AppendRadioItem(wxID_ANY, _("Use Layout Zoom"));
  m_tools_tiny = m_tools_menu->AppendRadioItem(wxID_ANY, _("Tiny"));
  m_tools_small = m_tools_menu->AppendRadioItem(wxID_ANY, _("Small"));
  m_tools_medium = m_tools_menu->AppendRadioItem(wxID_ANY, _("Medium"));
  m_tools_large = m_tools_menu->AppendRadioItem(wxID_ANY, _("Large"));
  m_tools_largest = m_tools_menu->AppendRadioItem(wxID_ANY, _("Largest"));
  m_tools_custom = m_tools_menu->AppendRadioItem(wxID_ANY, _("Custom Size"));
  m_tools_menu->AppendSeparator();
  m_tools_handle_navigation = m_tools_menu->AppendCheckItem(wxID_ANY, _("Handle Navigation"));
  m_tools_handle_new_window = m_tools_menu->AppendCheckItem(wxID_ANY, _("Handle New Windows"));
  m_tools_menu->AppendSeparator();

  //Find
  m_find = m_tools_menu->Append(wxID_ANY, _("Find"));
  m_tools_menu->AppendSeparator();

  //History menu
  m_tools_history_menu = new wxMenu();
  wxMenuItem* clearhist = m_tools_history_menu->Append(wxID_ANY, _("Clear History"));
  m_tools_enable_history = m_tools_history_menu->AppendCheckItem(wxID_ANY, _("Enable History"));
  m_tools_history_menu->AppendSeparator();

  m_tools_menu->AppendSubMenu(m_tools_history_menu, "History");

  //Create an editing menu
  wxMenu* editmenu = new wxMenu();
  m_edit_cut = editmenu->Append(wxID_ANY, _("Cut"));
  m_edit_copy = editmenu->Append(wxID_ANY, _("Copy"));
  m_edit_paste = editmenu->Append(wxID_ANY, _("Paste"));
  editmenu->AppendSeparator();
  m_edit_undo = editmenu->Append(wxID_ANY, _("Undo"));
  m_edit_redo = editmenu->Append(wxID_ANY, _("Redo"));
  editmenu->AppendSeparator();
  m_edit_mode = editmenu->AppendCheckItem(wxID_ANY, _("Edit Mode"));

  m_tools_menu->AppendSeparator();
  m_tools_menu->AppendSubMenu(editmenu, "Edit");

  wxMenu* scroll_menu = new wxMenu;
  m_scroll_line_up = scroll_menu->Append(wxID_ANY, "Line &up");
  m_scroll_line_down = scroll_menu->Append(wxID_ANY, "Line &down");
  m_scroll_page_up = scroll_menu->Append(wxID_ANY, "Page u&p");
  m_scroll_page_down = scroll_menu->Append(wxID_ANY, "Page d&own");
  m_tools_menu->AppendSubMenu(scroll_menu, "Scroll");

  wxMenu* script_menu = new wxMenu;
  m_script_string = script_menu->Append(wxID_ANY, "Return String");
  m_script_integer = script_menu->Append(wxID_ANY, "Return integer");
  m_script_double = script_menu->Append(wxID_ANY, "Return double");
  m_script_bool = script_menu->Append(wxID_ANY, "Return bool");
  m_script_object = script_menu->Append(wxID_ANY, "Return JSON object");
  m_script_array = script_menu->Append(wxID_ANY, "Return array");
  m_script_dom = script_menu->Append(wxID_ANY, "Modify DOM");
  m_script_undefined = script_menu->Append(wxID_ANY, "Return undefined");
  m_script_null = script_menu->Append(wxID_ANY, "Return null");
  m_script_date = script_menu->Append(wxID_ANY, "Return Date");
#if wxUSE_WEBVIEW_IE
  if (!wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
  {
   m_script_object_el = script_menu->Append(wxID_ANY, "Return JSON object changing emulation level");
   m_script_date_el = script_menu->Append(wxID_ANY, "Return Date changing emulation level");
   m_script_array_el = script_menu->Append(wxID_ANY, "Return array changing emulation level");
  }
#endif
  m_script_async = script_menu->Append(wxID_ANY, "Return String async");
  m_script_message = script_menu->Append(wxID_ANY, "Send script message");
  m_script_custom = script_menu->Append(wxID_ANY, "Custom script");
  m_tools_menu->AppendSubMenu(script_menu, _("Run Script"));
  wxMenuItem* addUserScript = m_tools_menu->Append(wxID_ANY, _("Add user script"));
  wxMenuItem* setCustomUserAgent = m_tools_menu->Append(wxID_ANY, _("Set custom user agent"));

  //Selection menu
  wxMenu* selection = new wxMenu();
  m_selection_clear = selection->Append(wxID_ANY, _("Clear Selection"));
  m_selection_delete = selection->Append(wxID_ANY, _("Delete Selection"));
  wxMenuItem* selectall = selection->Append(wxID_ANY, _("Select All"));

  editmenu->AppendSubMenu(selection, "Selection");

  wxMenuItem* loadscheme = m_tools_menu->Append(wxID_ANY, _("Custom Scheme Example"));
  wxMenuItem* usememoryfs = m_tools_menu->Append(wxID_ANY, _("Memory File System Example"));

  m_context_menu = m_tools_menu->AppendCheckItem(wxID_ANY, _("Enable Context Menu"));
  m_dev_tools = m_tools_menu->AppendCheckItem(wxID_ANY, _("Enable Dev Tools"));

  //By default we want to handle navigation and new windows
  m_tools_handle_navigation->Check();
  m_tools_handle_new_window->Check();
  m_tools_enable_history->Check();

  //Zoom
  m_zoomFactor = 100;
  m_tools_medium->Check();

  if (!m_pBrowser->CanSetZoomType(wxWEBVIEW_ZOOM_TYPE_LAYOUT))
   m_tools_layout->Enable(false);

  // Connect the toolbar events
  Bind(wxEVT_TOOL, &FrameWeb::OnBack, this, m_toolbar_back->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnForward, this, m_toolbar_forward->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnStop, this, m_toolbar_stop->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnReload, this, m_toolbar_reload->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnToolsClicked, this, m_toolbar_tools->GetId());

  Bind(wxEVT_TEXT_ENTER, &FrameWeb::OnUrl, this, m_url->GetId());

  // Connect find toolbar events.
  Bind(wxEVT_TOOL, &FrameWeb::OnFindDone, this, m_find_toolbar_done->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnFindText, this, m_find_toolbar_next->GetId());
  Bind(wxEVT_TOOL, &FrameWeb::OnFindText, this, m_find_toolbar_previous->GetId());

  // Connect find control events.
  Bind(wxEVT_TEXT, &FrameWeb::OnFindText, this, m_find_ctrl->GetId());
  Bind(wxEVT_TEXT_ENTER, &FrameWeb::OnFindText, this, m_find_ctrl->GetId());

  // Connect the webview events
  Bind(wxEVT_WEBVIEW_NAVIGATING, &FrameWeb::OnNavigationRequest, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_NAVIGATED, &FrameWeb::OnNavigationComplete, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_LOADED, &FrameWeb::OnDocumentLoaded, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_ERROR, &FrameWeb::OnError, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_NEWWINDOW, &FrameWeb::OnNewWindow, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_TITLE_CHANGED, &FrameWeb::OnTitleChanged, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_FULLSCREEN_CHANGED, &FrameWeb::OnFullScreenChanged, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FrameWeb::OnScriptMessage, this, m_pBrowser->GetId());
  Bind(wxEVT_WEBVIEW_SCRIPT_RESULT, &FrameWeb::OnScriptResult, this, m_pBrowser->GetId());

  // Connect the menu events
  Bind(wxEVT_MENU, &FrameWeb::OnSetPage, this, setPage->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnViewSourceRequest, this, viewSource->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnViewTextRequest, this, viewText->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnPrint, this, print->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnZoomLayout, this, m_tools_layout->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_tiny->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_small->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_medium->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_large->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_largest->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetZoom, this, m_tools_custom->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnClearHistory, this, clearhist->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnEnableHistory, this, m_tools_enable_history->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnCut, this, m_edit_cut->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnCopy, this, m_edit_copy->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnPaste, this, m_edit_paste->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnUndo, this, m_edit_undo->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRedo, this, m_edit_redo->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnMode, this, m_edit_mode->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnScrollLineUp, this, m_scroll_line_up->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnScrollLineDown, this, m_scroll_line_down->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnScrollPageUp, this, m_scroll_page_up->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnScrollPageDown, this, m_scroll_page_down->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptString, this, m_script_string->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptInteger, this, m_script_integer->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptDouble, this, m_script_double->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptBool, this, m_script_bool->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptObject, this, m_script_object->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptArray, this, m_script_array->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptDOM, this, m_script_dom->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptUndefined, this, m_script_undefined->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptNull, this, m_script_null->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptDate, this, m_script_date->GetId());
#if wxUSE_WEBVIEW_IE
  if (!wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
  {
   Bind(wxEVT_MENU, &FrameWeb::OnRunScriptObjectWithEmulationLevel, this, m_script_object_el->GetId());
   Bind(wxEVT_MENU, &FrameWeb::OnRunScriptDateWithEmulationLevel, this, m_script_date_el->GetId());
   Bind(wxEVT_MENU, &FrameWeb::OnRunScriptArrayWithEmulationLevel, this, m_script_array_el->GetId());
  }
#endif
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptMessage, this, m_script_message->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptCustom, this, m_script_custom->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnRunScriptAsync, this, m_script_async->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnAddUserScript, this, addUserScript->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSetCustomUserAgent, this, setCustomUserAgent->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnClearSelection, this, m_selection_clear->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnDeleteSelection, this, m_selection_delete->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnSelectAll, this, selectall->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnLoadScheme, this, loadscheme->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnUseMemoryFS, this, usememoryfs->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnFind, this, m_find->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnEnableContextMenu, this, m_context_menu->GetId());
  Bind(wxEVT_MENU, &FrameWeb::OnEnableDevTools, this, m_dev_tools->GetId());

  //Connect the idle events
  Bind(wxEVT_IDLE, &FrameWeb::OnIdle, this);
 }

 FrameWeb::~FrameWeb()
 {
  delete m_tools_menu;
 }

 /**
   * Method that retrieves the current state from the web control and updates the GUI
   * the reflect this current state.
   */
 void FrameWeb::UpdateState()
 {
  m_toolbar->EnableTool(m_toolbar_back->GetId(), m_pBrowser->CanGoBack());
  m_toolbar->EnableTool(m_toolbar_forward->GetId(), m_pBrowser->CanGoForward());

  if (m_pBrowser->IsBusy())
  {
   m_toolbar->EnableTool(m_toolbar_stop->GetId(), true);
  }
  else
  {
   m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
  }

  SetTitle(m_pBrowser->GetCurrentTitle());
  m_url->SetValue(m_pBrowser->GetCurrentURL());
 }

 void FrameWeb::OnIdle(wxIdleEvent& WXUNUSED(evt))
 {
  if (m_pBrowser->IsBusy())
  {
   wxSetCursor(wxCURSOR_ARROWWAIT);
   m_toolbar->EnableTool(m_toolbar_stop->GetId(), true);
  }
  else
  {
   wxSetCursor(wxNullCursor);
   m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
  }
 }

 /**
   * Callback invoked when user entered an URL and pressed enter
   */
 void FrameWeb::OnUrl(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->LoadURL(m_url->GetValue());
  m_pBrowser->SetFocus();
  UpdateState();
 }

 /**
     * Callback invoked when user pressed the "back" button
     */
 void FrameWeb::OnBack(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->GoBack();
  UpdateState();
 }

 /**
   * Callback invoked when user pressed the "forward" button
   */
 void FrameWeb::OnForward(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->GoForward();
  UpdateState();
 }

 /**
   * Callback invoked when user pressed the "stop" button
   */
 void FrameWeb::OnStop(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Stop();
  UpdateState();
 }

 /**
   * Callback invoked when user pressed the "reload" button
   */
 void FrameWeb::OnReload(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Reload();
  UpdateState();
 }

 void FrameWeb::OnClearHistory(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->ClearHistory();
  UpdateState();
 }

 void FrameWeb::OnEnableHistory(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->EnableHistory(m_tools_enable_history->IsChecked());
  UpdateState();
 }

 void FrameWeb::OnCut(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Cut();
 }

 void FrameWeb::OnCopy(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Copy();
 }

 void FrameWeb::OnPaste(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Paste();
 }

 void FrameWeb::OnUndo(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Undo();
 }

 void FrameWeb::OnRedo(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Redo();
 }

 void FrameWeb::OnMode(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->SetEditable(m_edit_mode->IsChecked());
 }

 void FrameWeb::OnLoadScheme(wxCommandEvent& WXUNUSED(evt))
 {
  wxPathList pathlist;
  pathlist.Add(".");
  pathlist.Add("..");
  pathlist.Add("../help");
  pathlist.Add("../../../samples/help");

  wxString path = wxFileName(pathlist.FindValidPath("doc.zip")).GetAbsolutePath();
  //Under MSW we need to flip the slashes
  path.Replace("\\", "/");
  path = "wxfs:///" + path + ";protocol=zip/doc.htm";
  m_pBrowser->LoadURL(path);
 }

 void FrameWeb::OnUseMemoryFS(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->LoadURL("memory:page1.htm");
 }

 void FrameWeb::OnEnableContextMenu(wxCommandEvent& evt)
 {
  m_pBrowser->EnableContextMenu(evt.IsChecked());
 }

 void FrameWeb::OnEnableDevTools(wxCommandEvent& evt)
 {
  m_pBrowser->EnableAccessToDevTools(evt.IsChecked());
 }

 void FrameWeb::OnFind(wxCommandEvent& WXUNUSED(evt))
 {
  wxString value = m_pBrowser->GetSelectedText();
  if (value.Len() > 150)
  {
   value.Truncate(150);
  }
  m_find_ctrl->SetValue(value);
  if (!m_find_toolbar->IsShown()) {
   m_find_toolbar->Show(true);
   SendSizeEvent();
  }
  m_find_ctrl->SelectAll();
 }

 void FrameWeb::OnFindDone(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Find("");
  m_find_toolbar->Show(false);
  SendSizeEvent();
 }

 void FrameWeb::OnFindText(wxCommandEvent& evt)
 {
  int flags = 0;

  if (m_find_toolbar_wrap->IsChecked())
   flags |= wxWEBVIEW_FIND_WRAP;
  if (m_find_toolbar_wholeword->IsChecked())
   flags |= wxWEBVIEW_FIND_ENTIRE_WORD;
  if (m_find_toolbar_matchcase->IsChecked())
   flags |= wxWEBVIEW_FIND_MATCH_CASE;
  if (m_find_toolbar_highlight->IsChecked())
   flags |= wxWEBVIEW_FIND_HIGHLIGHT_RESULT;

  if (m_find_toolbar_previous->GetId() == evt.GetId())
   flags |= wxWEBVIEW_FIND_BACKWARDS;

  wxString find_text = m_find_ctrl->GetValue();
  long count = m_pBrowser->Find(find_text, flags);

  if (m_findText != find_text)
  {
   m_findCount = count;
   m_findText = find_text;
  }

  if (count != wxNOT_FOUND || find_text.IsEmpty())
  {
   m_find_ctrl->SetBackgroundColour(*wxWHITE);
  }
  else
  {
   m_find_ctrl->SetBackgroundColour(wxColour(255, 101, 101));
  }

  m_find_ctrl->Refresh();

  //Log the result, note that count is zero indexed.
  if (count != m_findCount)
  {
   count++;
  }
  wxLogMessage("Searching for:%s  current match:%li/%i", m_findText, count, m_findCount);
 }

 /**
   * Callback invoked when there is a request to load a new page (for instance
   * when the user clicks a link)
   */
 void FrameWeb::OnNavigationRequest(wxWebViewEvent& evt)
 {
  if (m_info->IsShown())
  {
   m_info->Dismiss();
  }

  wxLogMessage("%s", "Navigation request to '" + evt.GetURL() + "' (target='" +
   evt.GetTarget() + "')");

  //If we don't want to handle navigation then veto the event and navigation
  //will not take place, we also need to stop the loading animation
  if (!m_tools_handle_navigation->IsChecked())
  {
   evt.Veto();
   m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
  }
  else
  {
   UpdateState();
  }
 }

 /**
   * Callback invoked when a navigation request was accepted
   */
 void FrameWeb::OnNavigationComplete(wxWebViewEvent& evt)
 {
  wxLogMessage("%s", "Navigation complete; url='" + evt.GetURL() + "'");
  UpdateState();
 }

 /**
   * Callback invoked when a page is finished loading
   */
 void FrameWeb::OnDocumentLoaded(wxWebViewEvent& evt)
 {
  //Only notify if the document is the main frame, not a subframe
  if (evt.GetURL() == m_pBrowser->GetCurrentURL())
  {
   wxLogMessage("%s", "Document loaded; url='" + evt.GetURL() + "'");
  }
  UpdateState();
 }

 /**
   * On new window, we veto to stop extra windows appearing
   */
 void FrameWeb::OnNewWindow(wxWebViewEvent& evt)
 {
  wxString flag = " (other)";

  if (evt.GetNavigationAction() == wxWEBVIEW_NAV_ACTION_USER)
  {
   flag = " (user)";
  }

  wxLogMessage("%s", "New window; url='" + evt.GetURL() + "'" + flag);

  //If we handle new window events then just load them in this window as we
  //are a single window browser
  if (m_tools_handle_new_window->IsChecked())
   m_pBrowser->LoadURL(evt.GetURL());

  UpdateState();
 }

 void FrameWeb::OnTitleChanged(wxWebViewEvent& evt)
 {
  SetTitle(evt.GetString());
  wxLogMessage("%s", "Title changed; title='" + evt.GetString() + "'");
 }

 void FrameWeb::OnFullScreenChanged(wxWebViewEvent& evt)
 {
  wxLogMessage("Full screen changed; status = %d", evt.GetInt());
  ShowFullScreen(evt.GetInt() != 0);
 }

 void FrameWeb::OnScriptMessage(wxWebViewEvent& evt)
 {
  wxLogMessage("Script message received; value = %s, handler = %s", evt.GetString(), evt.GetMessageHandler());
 }

 void FrameWeb::OnScriptResult(wxWebViewEvent& evt)
 {
  if (evt.IsError())
   wxLogError("Async script execution failed: %s", evt.GetString());
  else
   wxLogMessage("Async script result received; value = %s", evt.GetString());
 }

 void FrameWeb::OnSetPage(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->SetPage
  (
   "<html><title>New Page</title>"
   "<body>Created using <tt>SetPage()</tt> method.</body></html>",
   wxString()
  );
 }

 /**
   * Invoked when user selects the "View Source" menu item
   */
 void FrameWeb::OnViewSourceRequest(wxCommandEvent& WXUNUSED(evt))
 {
  SourceViewDialog dlg(this, m_pBrowser->GetPageSource());
  dlg.ShowModal();
 }

 /**
  * Invoked when user selects the "View Text" menu item
  */
 void FrameWeb::OnViewTextRequest(wxCommandEvent& WXUNUSED(evt))
 {
  wxDialog textViewDialog(this, wxID_ANY, "Page Text",
   wxDefaultPosition, wxSize(700, 500),
   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
#if wxUSE_STC
  wxStyledTextCtrl* text = new wxStyledTextCtrl(&textViewDialog, wxID_ANY);
  text->SetText(m_pBrowser->GetPageText());
#else // !wxUSE_STC
  wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, m_pBrowser->GetPageText(),
   wxDefaultPosition, wxDefaultSize,
   wxTE_MULTILINE |
   wxTE_RICH |
   wxTE_READONLY);
#endif // wxUSE_STC/!wxUSE_STC
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text, 1, wxEXPAND);
  SetSizer(sizer);
  textViewDialog.ShowModal();
 }

 /**
   * Invoked when user selects the "Menu" item
   */
 void FrameWeb::OnToolsClicked(wxCommandEvent& WXUNUSED(evt))
 {
  if (m_pBrowser->GetCurrentURL() == "")
   return;

  m_edit_cut->Enable(m_pBrowser->CanCut());
  m_edit_copy->Enable(m_pBrowser->CanCopy());
  m_edit_paste->Enable(m_pBrowser->CanPaste());

  m_edit_undo->Enable(m_pBrowser->CanUndo());
  m_edit_redo->Enable(m_pBrowser->CanRedo());

  m_selection_clear->Enable(m_pBrowser->HasSelection());
  m_selection_delete->Enable(m_pBrowser->HasSelection());

  m_context_menu->Check(m_pBrowser->IsContextMenuEnabled());
  m_dev_tools->Check(m_pBrowser->IsAccessToDevToolsEnabled());

  //Firstly we clear the existing menu items, then we add the current ones
  wxMenuHistoryMap::const_iterator it;
  for (it = m_histMenuItems.begin(); it != m_histMenuItems.end(); ++it)
  {
   m_tools_history_menu->Destroy(it->first);
  }
  m_histMenuItems.clear();

  wxVector<wxSharedPtr<wxWebViewHistoryItem> > back = m_pBrowser->GetBackwardHistory();
  wxVector<wxSharedPtr<wxWebViewHistoryItem> > forward = m_pBrowser->GetForwardHistory();

  wxMenuItem* item;

  unsigned int i;
  for (i = 0; i < back.size(); i++)
  {
   item = m_tools_history_menu->AppendRadioItem(wxID_ANY, back[i]->GetTitle());
   m_histMenuItems[item->GetId()] = back[i];
   Bind(wxEVT_MENU, &FrameWeb::OnHistory, this, item->GetId());
  }

  wxString title = m_pBrowser->GetCurrentTitle();
  if (title.empty())
   title = "(untitled)";
  item = m_tools_history_menu->AppendRadioItem(wxID_ANY, title);
  item->Check();

  //No need to connect the current item
  m_histMenuItems[item->GetId()] = wxSharedPtr<wxWebViewHistoryItem>(new wxWebViewHistoryItem(m_pBrowser->GetCurrentURL(), m_pBrowser->GetCurrentTitle()));

  for (i = 0; i < forward.size(); i++)
  {
   item = m_tools_history_menu->AppendRadioItem(wxID_ANY, forward[i]->GetTitle());
   m_histMenuItems[item->GetId()] = forward[i];
   Bind(wxEVT_TOOL, &FrameWeb::OnHistory, this, item->GetId());
  }

  wxPoint position = ScreenToClient(wxGetMousePosition());
  PopupMenu(m_tools_menu, position.x, position.y);
 }

 /**
   * Invoked when user selects the zoom size in the menu
   */
 void FrameWeb::OnSetZoom(wxCommandEvent& evt)
 {
  if (evt.GetId() == m_tools_tiny->GetId())
  {
   m_pBrowser->SetZoom(wxWEBVIEW_ZOOM_TINY);
  }
  else if (evt.GetId() == m_tools_small->GetId())
  {
   m_pBrowser->SetZoom(wxWEBVIEW_ZOOM_SMALL);
  }
  else if (evt.GetId() == m_tools_medium->GetId())
  {
   m_pBrowser->SetZoom(wxWEBVIEW_ZOOM_MEDIUM);
  }
  else if (evt.GetId() == m_tools_large->GetId())
  {
   m_pBrowser->SetZoom(wxWEBVIEW_ZOOM_LARGE);
  }
  else if (evt.GetId() == m_tools_largest->GetId())
  {
   m_pBrowser->SetZoom(wxWEBVIEW_ZOOM_LARGEST);
  }
  else if (evt.GetId() == m_tools_custom->GetId())
  {
   m_zoomFactor = m_pBrowser->GetZoomFactor() * 100;
   OnZoomCustom(evt);
  }
  else
  {
   wxFAIL;
  }
 }

 void FrameWeb::OnZoomLayout(wxCommandEvent& WXUNUSED(evt))
 {
  if (m_tools_layout->IsChecked())
   m_pBrowser->SetZoomType(wxWEBVIEW_ZOOM_TYPE_LAYOUT);
  else
   m_pBrowser->SetZoomType(wxWEBVIEW_ZOOM_TYPE_TEXT);
 }

 void FrameWeb::OnZoomCustom(wxCommandEvent& WXUNUSED(evt))
 {
  wxNumberEntryDialog dialog
  (
   this,
   "Enter zoom factor as a percentage (10-10000)%",
   "Zoom Factor:",
   "Change Zoom Factor",
   m_zoomFactor,
   10, 10000
  );
  if (dialog.ShowModal() != wxID_OK)
   return;

  m_zoomFactor = dialog.GetValue();
  m_pBrowser->SetZoomFactor((float)m_zoomFactor / 100);
 }

 void FrameWeb::OnHistory(wxCommandEvent& evt)
 {
  m_pBrowser->LoadHistoryItem(m_histMenuItems[evt.GetId()]);
 }

 void FrameWeb::RunScript(const wxString& javascript)
 {
  // Remember the script we run in any case, so the next time the user opens
  // the "Run Script" dialog box, it is shown there for convenient updating.
  m_javascript = javascript;

  wxLogMessage("Running JavaScript:\n%s\n", javascript);

  wxString result;
  if (m_pBrowser->RunScript(javascript, &result))
  {
   wxLogMessage("RunScript() returned \"%s\"", result);
  }
  else
  {
   wxLogWarning("RunScript() failed");
  }
 }

 void FrameWeb::OnRunScriptString(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(a){return a;}f('Hello World!');");
 }

 void FrameWeb::OnRunScriptInteger(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(a){return a;}f(123);");
 }

 void FrameWeb::OnRunScriptDouble(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(a){return a;}f(2.34);");
 }

 void FrameWeb::OnRunScriptBool(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(a){return a;}f(false);");
 }

 void FrameWeb::OnRunScriptObject(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(){var person = new Object();person.name = 'Foo'; \
        person.lastName = 'Bar';return person;}f();");
 }

 void FrameWeb::OnRunScriptArray(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(){ return [\"foo\", \"bar\"]; }f();");
 }

 void FrameWeb::OnRunScriptDOM(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("document.write(\"Hello World!\");");
 }

 void FrameWeb::OnRunScriptUndefined(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(){var person = new Object();}f();");
 }

 void FrameWeb::OnRunScriptNull(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(){return null;}f();");
 }

 void FrameWeb::OnRunScriptDate(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("function f(){var d = new Date('10/08/2017 21:30:40'); \
        var tzoffset = d.getTimezoneOffset() * 60000; \
        return new Date(d.getTime() - tzoffset);}f();");
 }

#if wxUSE_WEBVIEW_IE
 void FrameWeb::OnRunScriptObjectWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
 {
  wxWebViewIE::MSWSetModernEmulationLevel();
  RunScript("function f(){var person = new Object();person.name = 'Foo'; \
        person.lastName = 'Bar';return person;}f();");
  wxWebViewIE::MSWSetModernEmulationLevel(false);
 }

 void FrameWeb::OnRunScriptDateWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
 {
  wxWebViewIE::MSWSetModernEmulationLevel();
  RunScript("function f(){var d = new Date('10/08/2017 21:30:40'); \
        var tzoffset = d.getTimezoneOffset() * 60000; return \
        new Date(d.getTime() - tzoffset);}f();");
  wxWebViewIE::MSWSetModernEmulationLevel(false);
 }

 void FrameWeb::OnRunScriptArrayWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
 {
  wxWebViewIE::MSWSetModernEmulationLevel();
  RunScript("function f(){ return [\"foo\", \"bar\"]; }f();");
  wxWebViewIE::MSWSetModernEmulationLevel(false);
 }
#endif

 void FrameWeb::OnRunScriptMessage(wxCommandEvent& WXUNUSED(evt))
 {
  RunScript("window.wx.postMessage('This is a web message');");
 }

 void FrameWeb::OnRunScriptAsync(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->RunScriptAsync("function f(a){return a;}f('Hello World!');");
 }

 void FrameWeb::OnRunScriptCustom(wxCommandEvent& WXUNUSED(evt))
 {
  wxTextEntryDialog dialog
  (
   this,
   "Please enter JavaScript code to execute",
   wxGetTextFromUserPromptStr,
   m_javascript,
   wxOK | wxCANCEL | wxCENTRE | wxTE_MULTILINE
  );
  if (dialog.ShowModal() != wxID_OK)
   return;

  RunScript(dialog.GetValue());
 }

 void FrameWeb::OnAddUserScript(wxCommandEvent& WXUNUSED(evt))
 {
  wxString userScript = "window.wx_test_var = 'wxWidgets webview sample';";
  wxTextEntryDialog dialog
  (
   this,
   "Enter the JavaScript code to run as the initialization script that runs before any script in the HTML document.",
   wxGetTextFromUserPromptStr,
   userScript,
   wxOK | wxCANCEL | wxCENTRE | wxTE_MULTILINE
  );
  if (dialog.ShowModal() != wxID_OK)
   return;

  if (!m_pBrowser->AddUserScript(dialog.GetValue()))
   wxLogError("Could not add user script");
 }

 void FrameWeb::OnSetCustomUserAgent(wxCommandEvent& WXUNUSED(evt))
 {
  wxString customUserAgent = "Mozilla/5.0 (iPhone; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
  wxTextEntryDialog dialog
  (
   this,
   "Enter the custom user agent string you would like to use.",
   wxGetTextFromUserPromptStr,
   customUserAgent,
   wxOK | wxCANCEL | wxCENTRE
  );
  if (dialog.ShowModal() != wxID_OK)
   return;

  if (!m_pBrowser->SetUserAgent(customUserAgent))
   wxLogError("Could not set custom user agent");
 }

 void FrameWeb::OnClearSelection(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->ClearSelection();
 }

 void FrameWeb::OnDeleteSelection(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->DeleteSelection();
 }

 void FrameWeb::OnSelectAll(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->SelectAll();
 }

 /**
   * Callback invoked when a loading error occurs
   */
 void FrameWeb::OnError(wxWebViewEvent& evt)
 {
#define WX_ERROR_CASE(type) \
    case type: \
        category = #type; \
        break;

  wxString category;
  switch (evt.GetInt())
  {
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_CONNECTION);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_CERTIFICATE);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_AUTH);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_SECURITY);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_NOT_FOUND);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_REQUEST);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_USER_CANCELLED);
   WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_OTHER);
  }

  wxLogMessage("%s", "Error; url='" + evt.GetURL() + "', error='" + category + " (" + evt.GetString() + ")'");

  //Show the info bar with an error
  m_info->ShowMessage(_("An error occurred loading ") + evt.GetURL() + "\n" +
   "'" + category + "'", wxICON_ERROR);

  UpdateState();
 }

 /**
   * Invoked when user selects "Print" from the menu
   */
 void FrameWeb::OnPrint(wxCommandEvent& WXUNUSED(evt))
 {
  m_pBrowser->Print();
 }

 SourceViewDialog::SourceViewDialog(wxWindow* parent, wxString source) :
  wxDialog(parent, wxID_ANY, "Source Code",
   wxDefaultPosition, wxSize(700, 500),
   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
 {
#if wxUSE_STC
  wxStyledTextCtrl* text = new wxStyledTextCtrl(this, wxID_ANY);
  text->SetMarginWidth(1, 30);
  text->SetMarginType(1, wxSTC_MARGIN_NUMBER);
  text->SetText(source);

  text->StyleClearAll();
  text->SetLexer(wxSTC_LEX_HTML);
  text->StyleSetForeground(wxSTC_H_DOUBLESTRING, wxColour(255, 0, 0));
  text->StyleSetForeground(wxSTC_H_SINGLESTRING, wxColour(255, 0, 0));
  text->StyleSetForeground(wxSTC_H_ENTITY, wxColour(255, 0, 0));
  text->StyleSetForeground(wxSTC_H_TAG, wxColour(0, 150, 0));
  text->StyleSetForeground(wxSTC_H_TAGUNKNOWN, wxColour(0, 150, 0));
  text->StyleSetForeground(wxSTC_H_ATTRIBUTE, wxColour(0, 0, 150));
  text->StyleSetForeground(wxSTC_H_ATTRIBUTEUNKNOWN, wxColour(0, 0, 150));
  text->StyleSetForeground(wxSTC_H_COMMENT, wxColour(150, 150, 150));
#else // !wxUSE_STC
  wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, source,
   wxDefaultPosition, wxDefaultSize,
   wxTE_MULTILINE |
   wxTE_RICH |
   wxTE_READONLY);
#endif // wxUSE_STC/!wxUSE_STC

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text, 1, wxEXPAND);
  SetSizer(sizer);
 }

#endif

#endif
 }///namespace local