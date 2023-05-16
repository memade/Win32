#include "stdafx.h"
#include "export.h"

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long) {
 void* result = nullptr;
 do {

 } while (0);
 return result;
}

__shared_api_ void __stdcall api_object_uninit() {


}

__shared_api_ void* __stdcall api_server_init(const void*, unsigned long session_type) {
 void* result = nullptr;
 do {
  local::__gpServer = new local::Server(static_cast<SessionType>(session_type));
  result = reinterpret_cast<void*>(dynamic_cast<IServer*>(local::__gpServer));
 } while (0);
 return result;
}
__shared_api_ void __stdcall api_server_uninit() {
 if (local::__gpServer) {
  local::__gpServer->Stop();
  local::__gpServer->Release();
 }
}

__shared_api_ void* __stdcall api_client_init(const void*, unsigned long session_type) {
 void* result = nullptr;
 do {
  local::__gpClient = new local::Client(static_cast<SessionType>(session_type));
  result = reinterpret_cast<void*>(dynamic_cast<IClient*>(local::__gpClient));
 } while (0);
 return result;
}
__shared_api_ void __stdcall api_client_uninit() {
 if (local::__gpClient) {
  local::__gpClient->Stop();
  local::__gpClient->Release();
 }
}