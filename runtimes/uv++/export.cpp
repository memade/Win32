#include "stdafx.h"
using namespace local;

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long type) {
 void* result = nullptr;

 ServerType server_type = Protocol::GetServerType(type);
 SessionType session_type = Protocol::GetSessionType(type);
 AddressType address_type = Protocol::GetAddressType(type);
 if (server_type == ServerType::UNKNOWN || \
  session_type == SessionType::UNKNOWN || \
  address_type == AddressType::UNKNOWN)
  return result;

 switch (server_type) {
 case ServerType::ACCEPTOR: {
  result = reinterpret_cast<decltype(result)>(dynamic_cast<IService*>(new local::Server(type)));
 }break;
 case ServerType::INITIATOR: {
  result = reinterpret_cast<decltype(result)>(dynamic_cast<IService*>(new local::Client(type)));
 }break;
 default:
  break;
 }
 return result;
}

__shared_api_ void __stdcall api_object_uninit() {

}
