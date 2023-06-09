#include <win.h>
#include <encryption.hpp>
#include <res\help.txt.res.h>

int main(int argc, char** argv) {

 std::string show_text((char*)&help_txt_res[0], sizeof(help_txt_res));
 show_text = shared::Encryption::WemadeDecode(show_text);

 std::cout << show_text << std::endl;
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    exit = true;
   }
   else if (input == "1" || input == "/1" || input == "--1") {
    void(::_setmode(::_fileno(stdout), _O_WTEXT));
    //!@ C++ 头文件
    std::wstring seed = shared::Win::UuidW();
    std::string sign_time = shared::Win::Time::RFC1123();
    std::replace_if(seed.begin(), seed.end(), std::bind(std::equal_to<wchar_t>(), std::placeholders::_1, L'-'), L'_');
    std::locale unicode_local(std::locale{}, "", std::locale::ctype);
    std::locale prev_unicode_local = std::locale::global(unicode_local);
    //for (int i = 0; i < 1; ++i)
    // std::wcout << std::endl;
    //std::wcout << std::format(LR"(#if !defined(__{}__))", seed) << std::endl;
    //std::wcout << std::format(LR"(#define __{}__)", seed) << std::endl;
    //for (int i = 0; i < 3; ++i)
    // std::wcout << std::endl;
    //std::wcout << std::format(LR"(/// /*_ _{}{}_ _**/)",LR"(Memade®)",LR"(（新生™）)") << std::endl;
    //std::wcout << std::format(LR"(/// /*_ _{}_ _**/)", shared::IConv::MBytesToWString(sign_time)) << std::endl;
    //std::wcout << std::format(LR"(/// /*_ _ _ _{}_ _ _ _**/)",LR"(https://www.skstu.com/)") << std::endl;
    //std::wcout << std::format(LR"(#endif///__{}__)", seed) << std::endl;
    std::wstring contend = std::format(
     LR"(
#if !defined(__{}__)
#define __{}__


/// /*_ Memade®（新生™） _**/
/// /*_ {} _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__{}__
)", seed, seed, shared::IConv::MBytesToWString(sign_time), seed);
    std::wcout << contend << std::endl;

    std::locale::global(prev_unicode_local);
    void(::_setmode(::_fileno(stdout), _O_TEXT));
   }
   else if (input == "2" || input == "/2" || input == "--2") {

   }
   else if (input == "3" || input == "/3" || input == "--3") {

   }
  }, true);


 return 0;
}
