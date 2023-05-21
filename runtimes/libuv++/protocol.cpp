#include "stdafx.h"

namespace local {

 Protocol::Protocol() {

 }

 Protocol::~Protocol() {

 }
 bool Protocol::unmake_sock_session_identify(const TypeIdentify& identify, SessionType& type, std::string& ip, u_short& port) {
  bool result = false;
  ip.clear();
  port = 0;
  type = SessionType::Unknown;
  do {
   struct sockaddr addr = { 0 };
   if (!unmake_sock_session_identify(identify, type, addr))
    break;
   if (!parser_ipaddr((const sockaddr_storage*)&addr, ip, port))
    break;

   result = true;
  } while (0);
  return result;
 }
 std::string Protocol::unmake_sock_session_identify(const TypeIdentify& identify) {
  std::string result;
  do {
   SessionType _type = SessionType::Unknown;
   u_short _port = 0;
   std::string _ip;
   struct sockaddr addr = { 0 };
   if (!unmake_sock_session_identify(identify, _type, addr))
    break;
   if (!parser_ipaddr((const sockaddr_storage*)&addr, _ip, _port))
    break;
   result.append(_ip).append(":").append(std::to_string(_port));
  } while (0);
  return result;
 }
 bool Protocol::unmake_sock_session_identify(const TypeIdentify& identify, SessionType& type, struct sockaddr& addr) {
  bool result = false;
  type = SessionType::Unknown;
  memset(&addr, 0x00, sizeof(addr));
  do {
   if (identify <= 0)
    break;
   struct sockaddr_in* p_sockaddr_in = reinterpret_cast<struct sockaddr_in*>(&addr);
   type = static_cast<SessionType>(shared::shift_hex_by_nibbles(identify, 12, shared::ShiftDirection::RIGHT));
   p_sockaddr_in->sin_addr.S_un.S_addr = static_cast<decltype(p_sockaddr_in->sin_addr.S_un.S_addr)>(\
    shared::shift_hex_by_nibbles(identify, 4, shared::ShiftDirection::RIGHT) & 0x00000000FFFFFFFF);
   p_sockaddr_in->sin_port = static_cast<decltype(p_sockaddr_in->sin_port)>(identify & 0x000000000000FFFF);
   p_sockaddr_in->sin_family = AF_INET;
   result = true;
  } while (0);
  return result;
 }
 TypeIdentify Protocol::make_sock_session_identify(const SessionType& type, const struct sockaddr& addr) {
  TypeIdentify result = 0;
  do {
   if (addr.sa_family != AF_INET)
    break;
   const struct sockaddr_in* p_sockaddr_in = reinterpret_cast<const struct sockaddr_in*>(&addr);
   auto ip = static_cast<decltype(result)>(p_sockaddr_in->sin_addr.S_un.S_addr);
   auto port = static_cast<decltype(result)>(p_sockaddr_in->sin_port);
   auto session_type = static_cast<decltype(result)>(type);
   session_type = shared::shift_hex_by_nibbles(session_type, 12, shared::ShiftDirection::LEFT);
   ip = shared::shift_hex_by_nibbles(ip, 4, shared::ShiftDirection::LEFT);
   result = session_type | ip | port;
  } while (0);
  return result;
 }
 bool Protocol::unmake_pipe_session_identify(const TypeIdentify& identify, SessionType& type, time_t& create_time_ms) {
  bool result = false;
  do {
   if (identify <= 0)
    break;
   type = static_cast<SessionType>(shared::shift_hex_by_nibbles(identify, 12, shared::ShiftDirection::RIGHT));
   create_time_ms = 0x0000FFFFFFFFFFFF & identify;
   result = true;
  } while (0);
  return result;
 }
 TypeIdentify Protocol::make_pipe_session_identify(const SessionType&/*unsigned long*/ type) {
  TypeIdentify result = 0;
  do {
   auto session_type = static_cast<decltype(result)>(type);
   session_type = shared::shift_hex_by_nibbles(session_type, 12, shared::ShiftDirection::LEFT);
   result = session_type | shared::TimeStampUTC<std::chrono::milliseconds>();
  } while (0);
  return result;
 }
 std::string Protocol::MakeStream(const HEAD& input_head, const std::string& input_data) {
  std::string result;
  do {
   result.append((char*)&input_head, PACKET_HEAD_SIZE);
   PHEAD pHead = reinterpret_cast<PHEAD>(result.data());
   pHead->zip_type = static_cast<decltype(pHead->zip_type)>(ZipType::Unknown);
   pHead->encryption_type = static_cast<decltype(pHead->encryption_type)>(EncryptType::Unknown);
   pHead->data_size = input_data.size();
   pHead->original_size = pHead->data_size;
   do {
    if (pHead->original_size <= PACKET_COMPRESSION_STANDARD_SIZE)
     break;
    std::string zip;
    if (!shared::Zip::zipCompress(input_data, zip) || zip.empty())
     break;
    pHead->zip_type = static_cast<decltype(pHead->zip_type)>(ZipType::ZIP);
    pHead->data_size = zip.size();
    result.pop_back();
    result.append(zip);
    break;
   } while (0);
   if (ZipType(pHead->zip_type) != ZipType::Unknown)
    break;
   if (input_data.empty())
    break;
   result.pop_back();
   result.append(input_data);
  } while (0);
  if (!result.empty())
   ((PHEAD)result.data())->packet_size = result.size();
  return result;
 }
 bool Protocol::UnMakeStream(const std::string& input_data, HEAD& head, std::string& output_data) {
  bool result = false;
  memset(&head, 0x00, PACKET_HEAD_SIZE);
  output_data.clear();
  do {
   PHEAD pHead = (PHEAD)input_data.data();
   if (!pHead->Verify())
    break;
   memcpy(&head, pHead, PACKET_HEAD_SIZE);
   switch (ZipType(pHead->Zip())) {
   case ZipType::Unknown: {
    if (!pHead->DataSize())
     break;
    output_data.append(pHead->data, pHead->DataSize());
   }break;
   case ZipType::ZIP: {
    shared::Zip::zipUnCompress(pHead->data, pHead->OriginalSize(), output_data);
   }break;
   case ZipType::GZIP: {
    shared::Zip::gzipUnCompress(pHead->data, output_data);
   }break;
   default:
    break;
   }
   result = true;
  } while (0);
  return result;
 }
 bool Protocol::parser_ipaddr(const sockaddr_storage* in_addr, std::string& out_ip, u_short& out_port, const IPType& ipv) {
  bool result = false;
  out_ip.clear();
  out_port = 0;
  if (!in_addr)
   return result;
  int inet = (IPType::IPV6 == ipv) ? AF_INET6 : AF_INET;
  char ipv6[64] = { 0 };
  switch (inet) {
  case AF_INET6: {
   struct sockaddr_in6* addr6 = (struct sockaddr_in6*)in_addr;
   //低版本windows可能找不到inet_ntop函数。
#if _MSC_VER
   DWORD size = sizeof(ipv6);
   ::WSAAddressToStringA((LPSOCKADDR)addr6, sizeof(sockaddr_in6), NULL, ipv6, &size);
   out_ip = std::string(ipv6);
   auto index = out_ip.rfind(":");
   if (index >= 0) {
    out_ip.resize(index);
   }
   out_port = ::htons(addr6->sin6_port);
#else
   std::string str(::inet_ntop(inet, (void*)&(addr6->sin6_addr), ipv6, 64));
   out_ip.swap(str);
   out_port = ::htons(addr6->sin6_port);
#endif
   result = true;
  }break;
  case AF_INET: {
   struct sockaddr_in* addr4 = (struct sockaddr_in*)in_addr;
   std::string str(::inet_ntoa(addr4->sin_addr));
   out_ip.swap(str);
   out_port = ::htons(addr4->sin_port);

   result = true;
  }break;
  default:
   result = false;
   break;
  }
  return result;
 }
 bool Protocol::parser_ipaddr(const std::string& address, std::string& ip, u_short& port) {
  bool result = false;
  do {
   auto check = address.find(":");
   if (check == std::string::npos)
    break;
   std::vector<std::string> parser = shared::Win::StringSpiltA(address, ":");
   if (parser.size() != 2)
    break;
   ip = parser[0];
   port = atoi(parser[1].c_str());
   if (ip.empty() || port <= 0)
    break;
   result = true;
  } while (0);
  return result;
 }
 void Protocol::uv_close_default_loop() {
  auto default_loop_ = uv_default_loop();
  do {
   if (!default_loop_)
    break;
   uv_walk(default_loop_,
    [](uv_handle_t* handle, void* arg) {
     if (!uv_is_closing(handle))
      uv_close(handle, NULL);
    }, NULL);
   uv_run(default_loop_, UV_RUN_DEFAULT);
   uv_loop_close(default_loop_);
  } while (0);
 }
 void Protocol::uv_async_cb(uv_async_t* async_handle) {
  if (!async_handle)
   return;
  auto t_handle = T_GET_HANDLE_USERDATA(async_handle);
  switch (t_handle->AsyncReqType()) {
  case AsyncType::TYPE_CLOSE_HANDLE: {
   if (!t_handle->Route())
    break;
   uv_close(T_GET_HANDLE<uv_handle_t>(reinterpret_cast<UvHandle*>(t_handle->Route())),
    [](uv_handle_t* handle) {
     auto t_handle = reinterpret_cast<UvHandle*>(handle->data);
     t_handle->Status(HandleStatus::STATUS_CLOSED);
    });
  }break;
  case AsyncType::TYPE_WRITE: {
   UvWirte* write_req = reinterpret_cast<UvWirte*>(t_handle->Route());
   if (!write_req)
    break;
   int status = uv_write(
    &write_req->write,
    (uv_stream_t*)write_req->handle,
    &write_req->buf,
    1,
    [](uv_write_t* req, int status) {
     UvWirte* write_req = reinterpret_cast<UvWirte*>(req->data);
     if (write_req && write_req->write_cb)
      write_req->write_cb(write_req->route, status);
     SK_DELETE_PTR(write_req);
    });
   if (status == 0)
    break;
   if (write_req && write_req->write_cb)
    write_req->write_cb(write_req->route, status);
   SK_DELETE_PTR(write_req);
  }break;
  case AsyncType::TYPE_WRITE_UDP: {
   UvWirte* write_req = reinterpret_cast<UvWirte*>(t_handle->Route());
   if (!write_req)
    break;
   int status = uv_udp_send(
    &write_req->write_udp,
    (uv_udp_t*)write_req->handle,
    &write_req->buf,
    1,
    write_req->addr.sa_family ? &write_req->addr : NULL,
    [](uv_udp_send_t* req, int status) {
     UvWirte* write_req = reinterpret_cast<UvWirte*>(req->data);
     if (write_req && write_req->write_cb)
      write_req->write_cb(write_req->route, status);
     SK_DELETE_PTR(write_req);
    });
   if (status == 0)
    break;
   if (write_req && write_req->write_cb)
    write_req->write_cb(write_req->route, status);
   SK_DELETE_PTR(write_req);
  }break;
  default:
   break;
  }
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 tagPacketHeader::tagPacketHeader() {
  memset(this, 0x00, sizeof(*this));
  header_logo = 0xFAC9C2D0;
  footer_logo = 0xB4B4AAC1;
 }
 tagPacketHeader::tagPacketHeader(const CommandType& input_cmd) {
  memset(this, 0x00, sizeof(*this));
  header_logo = 0xFAC9C2D0;
  footer_logo = 0xB4B4AAC1;
  command_code = static_cast<decltype(command_code)>(input_cmd);
 }
 bool tagPacketHeader::Verify() const {
  return header_logo == 0xFAC9C2D0 && footer_logo == 0xB4B4AAC1;
 }
 ZipType tagPacketHeader::Zip() const {
  return ZipType(zip_type);
 }
 EncryptType tagPacketHeader::Encrypt() const {
  return EncryptType(encryption_type);
 }
 CommandType tagPacketHeader::Command() const {
  return CommandType(command_code);
 }
 unsigned long tagPacketHeader::DataSize() const {
  return data_size;
 }
 unsigned long tagPacketHeader::OriginalSize() const {
  return original_size;
 }
 unsigned long tagPacketHeader::PacketSize() const {
  return packet_size;
 }

 const std::uint64_t TIMEOUT_HEARBEAT_MS = 30000;
 const size_t PACKET_HEAD_SIZE = sizeof(HEAD);
 const size_t PACKET_COMPRESSION_STANDARD_SIZE = 512;
}///namespace local
