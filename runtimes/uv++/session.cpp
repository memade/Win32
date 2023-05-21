#include "stdafx.h"

namespace local {

 Session::Session() {
  m_pReadStream = new Stream(0xFFFF, StreamType::READ);
  m_pWriteStream = new Stream(0xFFFF, StreamType::WRITE);
 }

 Session::~Session() {
  SK_DELETE_PTR(m_pReadStream);
  SK_DELETE_PTR(m_pWriteStream);
 }
 void Session::Handle(uv_handle_t* handle) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  handle_ = handle;
 }
 uv_handle_t* Session::Handle() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return handle_;
 }
 void Session::Caller(void* caller) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pCaller = caller;
 }
 void* Session::Caller() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pCaller;
 }
 void Session::Route(void* route) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pRoute = route;
 }
 void* Session::Route() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pRoute;
 }
 void Session::Status(const SessionStatus& status) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Status = status;
 }
 const SessionStatus& Session::Status() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Status;
 }
 void Session::Address(const std::string& address) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Address = address;
 }
 const std::string& Session::Address() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Address;
 }
 void Session::SetReadBuf(uv_buf_t*& buf) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  buf->base = m_pReadStream->Base();
  buf->len = m_pReadStream->BufferSize();
 }
 void Session::SockAddr(const sockaddr_in& addr_in) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  memcpy(&m_SockAddr, &addr_in, sizeof(m_SockAddr));
 }
 void Session::SockAddr(const sockaddr& addr) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  memcpy(&m_SockAddr, &addr, sizeof(m_SockAddr));
 }
 void Session::ServerTypeSet(const ServerType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_ServerType = type;
 }
 const ServerType& Session::ServerTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_ServerType;
 }
 void Session::SessionTypeSet(const SessionType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_SessionType = type;
 }
 const SessionType& Session::SessionTypeGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_SessionType;
 }
 unsigned long long Session::ActivationTime(const unsigned long long& time_ms) {
  unsigned long long result = 0;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  result = m_ActivationTime.load() ? time_ms - m_ActivationTime.load() : 0;
  m_ActivationTime.store(time_ms);
  return result;
 }
 const sockaddr& Session::SockAddr() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_SockAddr;
 }
 std::string Session::Read() {
  std::string result = "";
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_pReadStream)
    break;
   if (!((*m_pReadStream) >> result))
    break;
  } while (0);
  return result;
 }
 bool Session::Read(const char* buf, const size_t& len) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!m_pReadStream)
    break;
   if (!buf || len <= 0)
    break;
   if (!(*m_pReadStream << std::string(buf, len)))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Session::Write(const CommandType& cmd, const std::string& original_data) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return *m_pWriteStream << Protocol::MakeStream(HEAD(CommandType(cmd)), original_data);
 }
 write_req_t* Session::Write() {
  write_req_t* result = nullptr;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_pWriteStream->Empty())
    break;
   std::string write_buffer;
   if (!(*m_pWriteStream >> write_buffer) || write_buffer.empty())
    break;
   result = new write_req_t;
   result->buf.len = static_cast<decltype(result->buf.len)>(write_buffer.size());
   result->buf.base = new char[result->buf.len];
   memcpy(result->buf.base, write_buffer.data(), result->buf.len);
  } while (0);
  return result;
 }

}///namespace local