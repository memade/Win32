#include "stdafx.h"

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long type) {
 void* result = nullptr;
 switch (ServerType(type)) {
 case ServerType::ACCEPTOR: {
  if (!local::__gpServer)
   local::__gpServer = new local::Server();
  local::__gpServer->ConfigGet()->Server(ServerType(type));
  result = reinterpret_cast<decltype(result)>(dynamic_cast<IServer*>(local::__gpServer));
 }break;
 case ServerType::INITIATOR: {
  if (!local::__gpClient)
   local::__gpClient = new local::Client();
  local::__gpClient->ConfigGet()->Server(ServerType(type));
  result = reinterpret_cast<decltype(result)>(dynamic_cast<IClient*>(local::__gpClient));
 }break;
 default:
  break;
 }
 return result;
}

__shared_api_ void __stdcall api_object_uninit() {
 if (local::__gpServer)
  local::__gpServer->Stop();
 if (local::__gpClient)
  local::__gpClient->Stop();

 SK_DELETE_PTR(local::__gpClient);
 SK_DELETE_PTR(local::__gpServer);
}
