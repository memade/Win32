#include "stdafx.h"

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long type) {
 void* result = nullptr;
 switch (ServerType(type)) {
 case ServerType::ACCEPTOR: {
  local::__gpServer = new local::Server();
  local::__gpServer->ConfigGet()->Server(ServerType(type));
  result = reinterpret_cast<decltype(result)>(dynamic_cast<IServer*>(local::__gpServer));
 }break;
 case ServerType::INITIATOR: {
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

}
