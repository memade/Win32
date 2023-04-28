#include "stdafx.h"

namespace local {

 Session::Session(const TypeIdentify& identify, const std::string& name, uv::TcpConnection* uv_connention) :
  m_Identify(identify),
  m_Name(name),
  m_pUvConnection(uv_connention) {

 }
 Session::~Session() {

 }
 void Session::Release() const {
  delete this;
 }
 const TypeIdentify& Session::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }
 const std::string& Session::Name() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Name;
 }
 void Session::BindTaskId(const unsigned long long& taskid) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_TaskId = taskid;
 }
 const unsigned long long& Session::BindTaskId() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_TaskId;
 }
 void Session::BindProcessId(const unsigned long& process) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_ProcessId = process;
 }
 const unsigned long& Session::BindProcessId() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_ProcessId;
 }
 void Session::BindUL(const unsigned long& ul) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BindUL = ul;
 }
 const unsigned long& Session::BindUL() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_BindUL;
 }
 void Session::BindULL(const unsigned long long& ull) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BindULL = ull;
 }
 const unsigned long long& Session::BindULL() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_BindULL;
 }
 void Session::BindStr(const std::string& str) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BindStr = str;
 }
 const std::string& Session::BindStr() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_BindStr;
 }
 void Session::BindPtr(void* ptr) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BindPtr = ptr;
 }
 void* Session::BindPtr() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_BindPtr;
 }
 void Session::Write() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_BufferWrite.empty())
    break;
   if (!m_pUvConnection)
    break;
   if (!m_pUvConnection->isConnected())
    break;
   std::string send_data = m_BufferWrite.pop();
   m_pUvConnection->write(send_data.data(), send_data.size(),
    [&](uv::WriteInfo write_info) {


     auto sk = 0;
    });
  } while (0);
 }
 bool Session::Ready() const {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_pUvConnection)
    break;
   if (!m_pUvConnection->isConnected())
    break;
   result = true;
  } while (0);
  return result;
 }
 void Session::Write(const std::string& input) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BufferWrite.push(input);
 }
 void Session::Write(const unsigned long long& cmd, const std::string& data) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  std::string send_data;
  shared::Win::Packet::Made(cmd, data, send_data);
  m_BufferWrite.push(send_data);
 }
 void Session::Read(const tfSessionReadCb& read_cb) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  read_cb(m_BufferRead.pop());
 }
 void Session::Read(std::string& output) {
  output.clear();
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  output = m_BufferRead.pop();
 }
 void Session::Read(std::vector<std::string>& out_data_s) {
  out_data_s.clear();
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_BufferRead.empty())
    break;
   std::string data = m_BufferRead.front();
   if (data.empty())
    break;
   size_t size_unpak = shared::Win::Packet::UnMade(data, out_data_s);
   if (size_unpak <= 0)
    break;
   m_BufferRead.pop(size_unpak);
  } while (0);
 }
 void Session::Read(const char* data, const size_t& size) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_BufferRead.push(data, size);
 }

}///namespace local
