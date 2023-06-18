#include <win.hpp>
#include <ssl.hpp>
using namespace ssl;

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto pSsl = ssl::ISsl::CreateInterface(R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\ssl++.dll)");
 assert(pSsl);

 pSsl->EnMD5(nullptr, 0, 
  [](const char* endata,size_t endata_size) {
  
   auto sk = 0;
  });

 std::string base64_encode, base64_decode;
 pSsl->EnBase64(
  "gaasgasssssssssssssssssssssssssssssssssssssshahadhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhasgasdggasdas", 
  strlen("gaasgasssssssssssssssssssssssssssssssssssssshahadhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhasgasdggasdas"), 
  true, 
  [&base64_encode](const char* data,size_t size) {
   base64_encode.append(data, size);
  });
 pSsl->DeBase64(base64_encode.data(), base64_encode.size(), true,
  [&base64_decode](const char* data, size_t size) {
   base64_decode.append(data, size);
  });

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    ssl::ISsl::DestoryInterface(pSsl);
    exit = true;
   }

  });


 return 0;
}

