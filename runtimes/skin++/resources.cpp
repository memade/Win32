#include "stdafx.h"

namespace local {

 Resources::Resources() {

 }
 Resources::~Resources() {
  for (auto& node : resources_)
   node.second->Release();
  resources_.clear();
 }
 void Resources::Release() const {
  delete this;
 }
 void Resources::Push(IResource* pRes) {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   if (!pRes)
    break;
   auto pAttributes = pRes->GetAttributes();
   if (!pAttributes) {
    SK_RELEASE_PTR(pRes);
    break;
   }
   auto pAttribute = pAttributes->GetAttribute(AttributeType::identify);
   if (!pAttribute) {
    SK_RELEASE_PTR(pRes);
    break;
   }

   auto id = pAttribute->GetIdentify();
   auto found = resources_.find(id);
   if (found != resources_.end()) {
    found->second->Release();
    resources_.erase(found);
   }

   resources_.emplace(pAttribute->GetIdentify(), pRes);
  } while (0);
 }
 void Resources::Clear() {
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto& node : resources_)
   node.second->Release();
  resources_.clear();
 }
 void Resources::Pop(const TypeIdentify& id) {
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   auto found = resources_.find(id);
   if (found == resources_.end())
    break;
   resources_.erase(found);
   found->second->Release();
  } while (0);
 }
 IResource* Resources::Search(const TypeIdentify& identify) const {
  IResource* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  auto found = resources_.find(identify);
  if (found != resources_.end())
   result = found->second;
  return result;
 }

}///namespace local