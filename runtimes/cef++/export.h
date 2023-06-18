#if !defined(__81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__)
#define __81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long);
__shared_api_ void __stdcall api_object_uninit();

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:26:50 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__
