﻿#include "stdafx.h"
using namespace local;

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long) {
 return reinterpret_cast<void*>(dynamic_cast<ICurl*>(new Curl()));
}

__shared_api_ void __stdcall api_object_uninit() {

}
