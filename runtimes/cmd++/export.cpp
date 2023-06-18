#include "stdafx.h"
using namespace local;

__shared_api_ void* __stdcall api_object_init(const void* arg_str, unsigned long arg_size) {
 return reinterpret_cast<void*>(dynamic_cast<cmd::ICmd*>(new Cmd()));
}

__shared_api_ void __stdcall api_object_uninit() {

}
