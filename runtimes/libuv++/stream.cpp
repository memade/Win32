#include "stdafx.h"

namespace local {

 Stream::Stream(const size_t& buffer_size, const StreamType& type) :
  m_BufferSize(__max(buffer_size, 512)),
  m_StreamType(type) {
  m_LevelCache1 = new char[m_BufferSize];
 }

 Stream::~Stream() {
  do {
   if (m_LevelCache3.empty())
    break;
   std::string p = m_LevelCache3.back();
   m_LevelCache3.pop();
   p.clear();
  } while (1);

  m_LevelCache2.clear();

  if (m_LevelCache1) {
   delete[] m_LevelCache1;
   m_LevelCache1 = nullptr;
  }
 }
 char* Stream::Base() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_LevelCache1;
 }
 bool Stream::operator<<(const std::string& input) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  const size_t size_input = input.size();
  switch (m_StreamType) {
  case StreamType::Read: {
   do {
    if (size_input <= 0)
     break;
    m_LevelCache2.append(input);
    size_t real_size_level_cache_2 = 0;
    result = true;
    HEAD* pHead = nullptr;
    do {
     real_size_level_cache_2 = m_LevelCache2.size();
     if (real_size_level_cache_2 < PACKET_HEAD_SIZE)
      break;
     pHead = reinterpret_cast<PHEAD>(m_LevelCache2.data());
     if (!pHead->Verify()) {
      result = false;
      break;
     }
     const size_t total = pHead->PacketSize();
     if (real_size_level_cache_2 < total)
      break;
     m_LevelCache3.push(m_LevelCache2.substr(0, total));
     m_LevelCache2.erase(m_LevelCache2.begin(), m_LevelCache2.begin() + total);
    } while (1);
   } while (0);
  }break;
  case StreamType::Write: {
   do {
    if (size_input < PACKET_HEAD_SIZE)
     break;
    m_LevelCache3.push(input);
    result = true;
   } while (0);
  }break;
  default:
   break;
  }
  return result;
 }
 bool Stream::operator>>(std::string& output) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  output.clear();
  switch (m_StreamType) {
  case StreamType::Read: {
   if (m_LevelCache3.empty())
    break;
   output = m_LevelCache3.back();
   m_LevelCache3.pop();
   result = true;
  }break;
  case StreamType::Write: {
   if (m_LevelCache3.empty())
    break;
   output = m_LevelCache3.back();
   m_LevelCache3.pop();
   result = true;
  }break;
  default:
   break;
  }
  return result;
 }
 bool Stream::Empty() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_LevelCache3.empty();
 }
 const size_t& Stream::BufferSize() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_BufferSize;
 }
 void Stream::Reset() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  if (m_LevelCache1 && m_BufferSize > 0)
  {
   delete[] m_LevelCache1;
   m_LevelCache1 = new char[m_BufferSize];
  }
  m_LevelCache2.clear();
  decltype(m_LevelCache3) empty;
  std::swap(empty, m_LevelCache3);
 }
 void Stream::Resize(const size_t& buffer_size) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (buffer_size <= 0)
    break;
   if (m_LevelCache1)
    delete[] m_LevelCache1;
   m_BufferSize = buffer_size;
   m_LevelCache1 = new char[m_BufferSize];
  } while (0);
 }

}///namespace local
