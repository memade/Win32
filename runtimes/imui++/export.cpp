﻿#include "stdafx.h"
using namespace local;

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long type) {
 return reinterpret_cast<void*>(dynamic_cast<imui::IImui*>(new Imui()));
}

__shared_api_ void __stdcall api_object_uninit() {

}
