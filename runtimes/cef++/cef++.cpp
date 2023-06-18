#include "stdafx.h"
#include <include/cef_browser.h>

namespace local {

 void ICefClient::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
  const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  if (hwnd)
   SetWindowText(hwnd, std::wstring(title).c_str());
 }


 Cef::Cef() {
  Init();
 }

 Cef::~Cef() {
  UnInit();
 }
 void Cef::Release() const {
  delete this;
 }
 void Cef::Init() {
  m_pConfig = new Config();
 }
 void Cef::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }
 IConfig* Cef::ConfigGet() const {
  return m_pConfig;
 }
 bool Cef::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_hMain = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, MainProcess, this, 0, NULL));
   if (!m_hMain)
    break;

   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }

 void Cef::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   ::WaitForSingleObject(m_hMain, INFINITE);
   SK_CLOSE_HANDLE(m_hMain);

   m_IsOpen.store(false);
  } while (0);
 }

 void Cef::Show(const bool& show) const {

 }
 HWND Cef::MainWnd() const {
  return nullptr;
 }
 HANDLE Cef::MainProcess() const {
  return nullptr;
 }
 HWND Cef::CreateFrameChild(const bool&) {
  return nullptr;
 }
 void Cef::RegisterOnDestroyCb(const tfOnDestroyCb&) {

 }
 unsigned int Cef::MainProcess(void* arg) {
  Cef* _This = reinterpret_cast<Cef*>(arg);

  int exit_code = 0;
#if 0//!@!!!32-bit cpus are not supported
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
  CefMainArgs main_args(__gpHinstance);
  // CEF applications have multiple sub-processes (render, GPU, etc) that share
  // the same executable. This function checks the command-line and, if this is
  // a sub-process, executes the appropriate logic.
  exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
  if (exit_code >= 0) {
   // The sub-process has completed so return here.
   return exit_code;
  }
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
  CefRefPtr<ICefApp> app(new ICefApp);
  // Initialize CEF.
  CefInitialize(main_args, settings, app.get(), sandbox_info);
  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();
  // Shut down CEF.
  CefShutdown();
  return 0;
 }


 extern HINSTANCE __gpHinstance = nullptr;
}///namespace local