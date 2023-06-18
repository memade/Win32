#if !defined(__51F54DFA_0E25_4B6F_AAA1_9D8507CDD181__)
#define __51F54DFA_0E25_4B6F_AAA1_9D8507CDD181__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long);
__shared_api_ void __stdcall api_object_uninit();

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 10 Jun 2023 11:05:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__51F54DFA_0E25_4B6F_AAA1_9D8507CDD181__

