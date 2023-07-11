#include "stdafx.h"

namespace local {

 Cache::Cache() {

 }
 Cache::Cache(const std::string& buffer) {
  *this << buffer;
 }
 Cache::Cache(const std::vector<char>& buffer) {
  *this << buffer;
 }
 Cache::~Cache() {
  SK_DELETE_PTR_BUFFER(buffer_);
 }
 void Cache::Release() const {
  delete this;
 }
 void Cache::operator=(const Cache& obj) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  SK_DELETE_PTR_BUFFER(buffer_);
  if (obj.buffer_ && obj.size_ > 0) {
   buffer_ = new char[obj.size_];
   memcpy(buffer_, obj.buffer_, obj.size_);
  }
 }
 void Cache::operator<<(const std::vector<char>& input) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  SK_DELETE_PTR_BUFFER(buffer_);
  size_ = 0;
  do {
   if (input.empty())
    break;
   size_ = input.size();
   buffer_ = new char[size_];
   memcpy(buffer_, &input[0], size_);
  } while (0);
 }
 void Cache::operator<<(const std::string& input) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  SK_DELETE_PTR_BUFFER(buffer_);
  size_ = 0;
  do {
   if (input.empty())
    break;
   size_ = input.size();
   buffer_ = new char[size_];
   memcpy(buffer_, input.data(), size_);
  } while (0);
 }
 const char* Cache::Buffer() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return buffer_;
 }

 const size_t& Cache::Size() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return size_;
 }

}///namespace local