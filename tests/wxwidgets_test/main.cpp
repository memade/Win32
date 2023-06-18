#include <win.hpp>
#if 1
#include <wxui.hpp>
using namespace wxui;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

#if 1//!@ TEST WIN :: Encryption

 auto adksf = shared::Win::Encryption::base64_encode_pem("#ASDGASDGA253245gsdfhsh4325266@#$&@$#%@#$%!");

 auto decode = shared::Win::Encryption::base64_decode(adksf);
 auto xxx = 0;

#endif



 auto pWxui = wxui::IWxui::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\wxui++.dll)");
 pWxui->RegisterOnAppCreateCb(
  [](IApp* app) {
   app->RegisterOnAppCreateFrameCb(
    [](IFrame* frame) {    
     if (!frame)
      return;
     frame->RegisterOnFrameCreateChildCb(
      [](auto child) {
       child->Show(true);
       child->Maximize();
       auto hwnd = child->Hwnd();
      
       auto xx = 0;
      });
     void(frame->CreateChildFrame());
    });
   auto sk = 0;
  });
 auto config = pWxui->ConfigGet();
 config->FrameType(wxui::EnFrameType::WEBVIEW);
 pWxui->Start();

 pWxui->CreateFrameChild(true);


 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    pWxui->Stop();
    wxui::IWxui::DestoryInterface(pWxui);
    exit = true;
   }

  });


 return 0;
}










#else

#include <wxskin.hpp>

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto wxui = new wx::IWxui(::GetModuleHandleW(nullptr));
 wxui->EnableExitConfirmation(false);
 wxui->Start();
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    wxui->Stop();
    wxui->Release();
    exit = true;
   }

  });


 return 0;
}
#endif