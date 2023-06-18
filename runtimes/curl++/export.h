#if !defined(__048377A6_6145_41AB_A8FC_6C05D6A93846__)
#define __048377A6_6145_41AB_A8FC_6C05D6A93846__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long);
__shared_api_ void __stdcall api_object_uninit();

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 05 Jun 2023 13:38:21 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__048377A6_6145_41AB_A8FC_6C05D6A93846__