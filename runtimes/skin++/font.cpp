#include "stdafx.h"

namespace local {

#if 0
 FontTmp::FontTmp() {

 }

 FontTmp::~FontTmp() {

 }
 const TypeIdentifyType& FontTmp::Identify() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return identify_;
 }
 const bool& FontTmp::Bold() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return bold_;
 }
 const ICache* FontTmp::GetCache() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return dynamic_cast<const ICache*>(&font_cache_);
 }
 void* FontTmp::Handle() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return nullptr;
 }
 const IString* FontTmp::FilePathname() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return dynamic_cast<const IString*>(&file_pathname_);
 }
#endif

}///namespace local