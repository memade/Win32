#include "stdafx.h"

namespace local {

 Cache::Cache(const std::string& cache_filepathname)
  : m_cache_filepathname(cache_filepathname) {
  Init();
 }

 Cache::~Cache() {
  UnInit();
 }

 void Cache::Release() const {
  delete this;
 }

 void Cache::Init() {
  do {
   const auto file_path = shared::Win::GetPathByPathnameA(m_cache_filepathname, false);
   if (!std::filesystem::exists(file_path)) {
    if (!std::filesystem::create_directories(file_path))
     break;
   }
   else {
    std::ifstream ifile(m_cache_filepathname, std::ifstream::ate | std::ifstream::binary);
    if (ifile.is_open()) {
     m_CurrentSize.store(ifile.tellg());
     ifile.close();
    }
   }

   m_pOfstream = new std::ofstream();
   m_pOfstream->open(m_cache_filepathname, m_openmode);
   if (!m_pOfstream->is_open()) {
    SK_DELETE_PTR(m_pOfstream);
    break;
   }
   m_pOfstream->close();

   m_Ready.store(true);
  } while (0);

 }

 void Cache::UnInit() {
  if (m_pOfstream && m_pOfstream->is_open())
   m_pOfstream->close();
  SK_DELETE_PTR(m_pOfstream);
 }
 bool Cache::Ready() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Ready.load();
 }
 std::string Cache::Get() const {
  std::string result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!shared::Win::AccessA(m_cache_filepathname))
    break;
   result = shared::Win::File::ReadCXX(m_cache_filepathname);
  } while (0);
  return result;
 }
 size_t Cache::Size() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_CurrentSize.load();
 }
 bool Cache::operator>>(std::string& output) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  output.clear();
  do {
   if (!shared::Win::AccessA(m_cache_filepathname))
    break;
   output = shared::Win::File::ReadCXX(m_cache_filepathname);
   result = true;
  } while (0);
  return result;
 }
 bool Cache::operator<<(const std::string& input) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (input.empty())
    break;
   if (!m_pOfstream)
    m_pOfstream = new std::ofstream;
   if (!m_pOfstream->is_open())
    m_pOfstream->open(m_cache_filepathname, m_openmode);
   if (!m_pOfstream->is_open())
    break;
   *m_pOfstream << input;
   m_pOfstream->close();
   m_CurrentSize += input.size();
   result = true;
  } while (0);
  return result;
 }

}///namespace local
