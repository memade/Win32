#include "stdafx.h"

namespace local {
 Resources::Resources(const ResourcesType& type) : type_(type) {

 }
 Resources::~Resources() {

 }
 void Resources::Release() const {
  delete this;
 }
 void Resources::Path(const char* input) {
  path_ = input ? input : "";
 }
 void Resources::Pathname(const char* input) {
  pathname_ = input ? input : "";
 }
 void Resources::Cache(const char* buffer, const size_t& len) {
  cache_.clear();
  cache_.append((char*)buffer, len);
 }
 const ResourcesType& Resources::Type() const {
  return type_;
 }
 const char* Resources::Path() const {
  return path_.c_str();
 }
 const char* Resources::Pathname() const {
  return pathname_.c_str();
 }
 void Resources::Cache(const tfCacheCb& cache_cb) const {
  cache_cb(cache_.c_str(), cache_.size());
 }
 const std::string& Resources::Cache() const {
  return cache_;
 }
 bool Resources::Load() {
  cache_ = shared::Win::File::ReadCXX(pathname_);
  return !cache_.empty();
 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ResourcesConfigXml::ResourcesConfigXml() : Resources(ResourcesType::CONFIG_XML){

 }
 ResourcesConfigXml::~ResourcesConfigXml() {

 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ResourcesConfigJson::ResourcesConfigJson() : Resources(ResourcesType::CONFIG_JSON) {

 }
 ResourcesConfigJson::~ResourcesConfigJson() {

 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ResourcesImageIcon::ResourcesImageIcon() : Resources(ResourcesType::IMAGE_ICON_) {

 }
 ResourcesImageIcon::~ResourcesImageIcon() {

 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ResourcesFontTTF::ResourcesFontTTF() : Resources(ResourcesType::FONT_TTF) {

 }
 ResourcesFontTTF::~ResourcesFontTTF() {

 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ResourcesFontTTC::ResourcesFontTTC() : Resources(ResourcesType::FONT_TTC) {

 }
 ResourcesFontTTC::~ResourcesFontTTC() {

 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
}///namespace local