﻿#if !defined(__D43B5F42_3AE0_4B20_9C76_832BF5F3E267__)
#define __D43B5F42_3AE0_4B20_9C76_832BF5F3E267__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace shared {
 using t_interface_init = void* (__stdcall*)(const void*, unsigned long);
 using t_interface_uninit = void(__stdcall*)(void);

 template<typename T>
 class InterfaceDll {
 protected:
  InterfaceDll() {}
  ~InterfaceDll() {}
 public:
  static T* CreateInterface(const std::string& module_pathname, \
   const void* route,
   const unsigned long& route_size,
   const std::string& __api_object_init = "api_object_init", const std::string& __api_object_uninit = "api_object_uninit") {
   T* result = nullptr;
   HMODULE hModule = nullptr;
   do {
    if (module_pathname.empty())
     break;
    hModule = ::LoadLibraryA(module_pathname.c_str());
    if (!hModule)
     break;
    auto api_object_init__ = \
     reinterpret_cast<decltype(T::api_object_init)>(::GetProcAddress(hModule, __api_object_init.c_str()));
    auto api_object_uninit__ = \
     reinterpret_cast<decltype(T::api_object_uninit)>(::GetProcAddress(hModule, __api_object_uninit.c_str()));
    if (!api_object_init__ || !api_object_uninit__)
     break;
    result = reinterpret_cast<decltype(result)>(api_object_init__(route, route_size));
    if (!result)
     break;
    result->hModule = hModule;
    result->api_object_init = api_object_init__;
    result->api_object_uninit = api_object_uninit__;
   } while (0);
   if (nullptr == result && hModule != nullptr) {
    ::FreeLibrary(hModule);
    hModule = nullptr;
   }
   return result;
  }
  static void DestoryInterface(T*& instance) {
   do {
    if (!instance)
     break;
    if (!instance->hModule || !instance->api_object_uninit)
     break;
    HMODULE freeMod = reinterpret_cast<HMODULE>(instance->hModule);
    instance->api_object_uninit();
    instance = nullptr;
    ::FreeLibrary(freeMod);
    freeMod = nullptr;
   } while (0);
  }
 protected:
  void* hModule = nullptr;
  t_interface_init api_object_init = nullptr;
  t_interface_uninit api_object_uninit = nullptr;
 };

}///namespace shared



/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Dec 2022 05:38:48 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__D43B5F42_3AE0_4B20_9C76_832BF5F3E267__