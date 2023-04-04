﻿#include "stdafx.h"

namespace wx {

 IwxMDIChildFrameCef::IwxMDIChildFrameCef(wxMDIParentFrame* parent,
  wxWindowID id /*= wxID_ANY*/,
  const wxString& title /*= L""*/,
  const wxPoint& pos /*= wxDefaultPosition*/,
  const wxSize& size /*= wxDefaultSize*/,
  long style /*= wxDEFAULT_FRAME_STYLE*/,
  const wxString& name /*= wxASCII_STR(wxFrameNameStr)*/) :
  wxMDIChildFrame(parent, id, title, pos, size, style, name) {

#if WX_ENABLE_MODULE_CEF

#if defined(ARCH_CPU_32_BITS)
  // Run the main thread on 32-bit Windows using a fiber with the preferred 4MiB
  // stack size. This function must be called at the top of the executable entry
  // point function (`main()` or `wWinMain()`). It is used in combination with
  // the initial stack size of 0.5MiB configured via the `/STACK:0x80000` linker
  // flag on executable targets. This saves significant memory on threads (like
  // those in the Windows thread pool, and others) whose stack size can only be
  // controlled via the linker flag.
  exit_code = CefRunWinMainWithPreferredStackSize(wWinMain, hInstance,
   lpCmdLine, nCmdShow);
  if (exit_code >= 0) {
   // The fiber has completed so return here.
   return exit_code;
  }
#endif

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(::GetModuleHandleW(NULL));
  // CEF applications have multiple sub-processes (render, GPU, etc) that share
  // the same executable. This function checks the command-line and, if this is
  // a sub-process, executes the appropriate logic.
  auto exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
  // Parse command-line arguments for use in this method.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromString(::GetCommandLineW());
  // Specify CEF global settings here.
  CefSettings settings;
  if (command_line->HasSwitch("enable-chrome-runtime")) {
   // Enable experimental Chrome runtime. See issue #2969 for details.
   settings.chrome_runtime = true;
  }
#if !defined(CEF_USE_SANDBOX)
  settings.no_sandbox = true;
#endif
  // SimpleApp implements application-level callbacks for the browser process.
  // It will create the first browser instance in OnContextInitialized() after
  // CEF has initialized.
  CefRefPtr<cef::ICefApp> app(new cef::ICefApp);
  // Initialize CEF.
  CefInitialize(main_args, settings, app.get(), sandbox_info);
#endif

  Bind(wxEVT_SIZE, &IwxMDIChildFrameCef::OnSize, this);
  Bind(wxEVT_MOVE, &IwxMDIChildFrameCef::OnMove, this);
  Bind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameCef::OnCloseWindow, this);
 }

 IwxMDIChildFrameCef::~IwxMDIChildFrameCef() {

  Unbind(wxEVT_SIZE, &IwxMDIChildFrameCef::OnSize, this);
  Unbind(wxEVT_MOVE, &IwxMDIChildFrameCef::OnMove, this);
  Unbind(wxEVT_CLOSE_WINDOW, &IwxMDIChildFrameCef::OnCloseWindow, this);
 }

 void IwxMDIChildFrameCef::OnSize(wxSizeEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameCef::OnMove(wxMoveEvent& wxEvent) {
  wxEvent.Skip();
 }
 void IwxMDIChildFrameCef::OnCloseWindow(wxCloseEvent& wxEvent) {

  wxEvent.Skip();
 }
}///namespace wx