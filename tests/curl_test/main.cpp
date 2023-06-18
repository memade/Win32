#include <win.hpp>
#include <curl.hpp>
using namespace curl;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pCurl = ICurl::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\curl++.dll)");
 auto config = pCurl->ConfigGet();
 pCurl->Start();
 pCurl->RegisterRequestBeginCb(
  [](const IRequest* reqObj) {

   auto sksksk = 0;
  });
 pCurl->RegisterRequestDoneCb(
  [](const IRequest* reqObj) {

   auto sksksk = 0;
  });

 

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    pCurl->Stop();
    curl::ICurl::DestoryInterface(pCurl);
    exit = true;
   }
   else if (input == "test") {
    auto req = pCurl->CreateRequest(R"(https://sm.myapp.com/original/Input/wbpy-7.9.exe)");
    //!@ Î¢ÈíÎå±Ê
    //! R"(https://sm.myapp.com/original/Input/wbpy-7.9.exe)"
    //!@ QQÎå±Ê
    //req->Url(R"(https://c30edfd09a11dbd65b0af09a230c4e9b.dlied1.cdntips.net/dl.softmgr.qq.com/original/Input/QQWubi_Setup_2.4.629.400.exe?mkey=647fccd47675134e&f=0000&cip=118.117.53.187&proto=https)");
    //req->Url(R"(https://cn.bing.com/)");
    //req->Url(R"(https://cef-builds.spotifycdn.com/cef_binary_114.2.8%2Bg8746aab%2Bchromium-114.0.5735.91_windows32.tar.bz2)");
    //req->Url(R"(https://down10.zol.com.cn/bofang/QianqianMusicSetup11.1.6.zip)");
    req->Type(static_cast<TypeRequestType>(RequestType::DOWN) | static_cast<TypeRequestType>(RequestType::HEAD));
    req->EnableWriteCache(true);
    req->Verbose(true);
    req->Start();
   }
  });


 return 0;
}
