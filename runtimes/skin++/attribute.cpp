#include "stdafx.h"

namespace local {
 Attributes::Attributes(const String& skin_root_path, const ControlType& control_type)
  : control_type_(control_type)
  , skin_root_path_(skin_root_path) {

 }
 Attributes:: ~Attributes() {
  for (auto& it : attributes_)
   SK_RELEASE_PTR(it);
  attributes_.clear();
 }
 void Attributes::Release() const {
  delete this;
 }
 void Attributes::operator<<(const Attribute& attribute) {
  *this << dynamic_cast<const IAttribute*>(new Attribute(attribute));
 }
 void Attributes::operator<<(const IAttribute* input) {
  do {
   if (!input)
    break;
   for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
    if ((*it)->GetType() == input->GetType()) {
     (*it)->Release();
     attributes_.erase(it);
     break;
    }
   }
   attributes_.emplace_back(const_cast<IAttribute*>(input));
  } while (0);
 }
 const ControlType& Attributes::GetControlType() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return control_type_;
 }
 bool Attributes::Empty() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return attributes_.empty();
 }
 Vec4 Attributes::GetBkColor(const AttributeType& type) const {
  Vec4 result;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
   if (type == (*it)->GetType()) {
    result = (*it)->GetVec4();
    break;
   }
  }
  return result;
 }
 const IString* Attributes::GetTitleName(const AttributeType& type) const {
  const IString* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
   if (type == (*it)->GetType()) {
    result = (*it)->GetString();
    break;
   }
  }
  return result;
 }
 Vec2 Attributes::GetSize(const AttributeType& type) const {
  Vec2 result;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
   if (type == (*it)->GetType()) {
    result = (*it)->GetVec2();
    break;
   }
  }
  return result;
 }
 const ICache* Attributes::GetCache(const AttributeType& type) const {
  const ICache* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
   if (type == (*it)->GetType()) {
    result = (*it)->GetCache();
    break;
   }
  }
  return result;
 }
 void Attributes::Iterator(const std::function<void(const AttributeType&, IAttribute*, bool&)>& iterator_cb) const {
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto& it : attributes_) {
   bool __break = false;
   iterator_cb(it->GetType(), it, __break);
   if (__break)
    break;
  }
 }
 const IAttribute* Attributes::GetAttribute(const AttributeType& type) const {
  const IAttribute* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
   if ((*it)->GetType() == type) {
    result = *it;
    break;
   }
  }
  return result;
 }
 void Attributes::operator<<(const std::map<AttributeType, String>& input_attribute_s) {
  std::lock_guard<std::mutex>(*m_Mutex);
  for (const auto& node : input_attribute_s) {
   Attribute* attribute = new Attribute(control_type_, node.first);
   attribute->SetSkinRootPath(skin_root_path_);
   if (!attribute->SetValue(node.second))
    SK_RELEASE_PTR(attribute);
   if (attribute) {
    *this << dynamic_cast<const IAttribute*>(attribute);
   }
  }
 }
 void Attributes::Push(const Attribute& input) {
  std::lock_guard<std::mutex>(*m_Mutex);
  *this << input;
 }
 void Attributes::Push(const IAttribute* input) {
  std::lock_guard<std::mutex>(*m_Mutex);
  *this << input;
 }
 unsigned long Attributes::Total() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return static_cast<unsigned long>(attributes_.size());
 }
 const IAttribute* Attributes::Next(const unsigned long& idx) const {
  const IAttribute* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  do {
   if (!(attributes_.size() > idx))
    break;
   result = attributes_[idx];
  } while (0);
  return result;
 }
 Attribute::Attribute(const Attribute& obj)
  : attribute_type_(obj.attribute_type_)
  , control_type_(obj.control_type_)
  , value_(obj.value_) {

 }
 Attribute::Attribute(const ControlType& control_type, const AttributeType& attribute_type, const tfAttributeValueType& value)
  : attribute_type_(attribute_type)
  , control_type_(control_type)
  , value_(value) {

 }
 Attribute::Attribute(const ControlType& control_type, const AttributeType& attribute_type)
  : attribute_type_(attribute_type)
  , control_type_(control_type) {
 }
 Attribute::~Attribute() {
  if (std::holds_alternative<IString*>(value_))
   SK_RELEASE_PTR(std::get<IString*>(value_));
  if (std::holds_alternative<ICache*>(value_))
   SK_RELEASE_PTR(std::get<ICache*>(value_));
 }
 void Attribute::SetSkinRootPath(const String& path) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  skin_root_path_ = path;
 }
 void Attribute::operator=(const Attribute& obj) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (control_type_ != obj.control_type_)
    break;
   if (attribute_type_ != obj.attribute_type_)
    break;
   skin_root_path_ = obj.skin_root_path_;
   value_ = obj.value_;
  } while (0);
 }
 void Attribute::operator<<(const tfAttributeValueType& value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  value_ = value;
 }
 bool Attribute::SetValue(const String& input_value) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  switch (attribute_type_) {
   //!@ float
  case AttributeType::width:
   [[fallthrough]];
  case AttributeType::height:
   [[fallthrough]];
  case AttributeType::alpha: {
   if (input_value.empty())
    break;
   value_ = wcstof(input_value.wstr(), nullptr);
   result = true;
  }break;
   //!@ id
  case AttributeType::identify: {
   if (input_value.empty())
    break;
   value_ = wcstoul(input_value.wstr(), nullptr, 10);
   result = true;
  }break;
   //!@ file cache
  case AttributeType::file: {
   if (input_value.empty())
    break;
   String file_pathname;
   if (!shared::Win::AccessW(input_value.wstr())) {
    file_pathname = shared::Win::PathFixedW(skin_root_path_.toStringW() + L"\\" + input_value.toStringW());
   }
   if (!shared::Win::AccessW(file_pathname.wstr()))
    break;
   std::vector<char> buffer;
   shared::Win::File::ReadCXX(file_pathname.str(), buffer);
   if (buffer.empty())
    break;
   value_ = dynamic_cast<ICache*>(new Cache(buffer));
   result = true;
  }break;
   //!@ String
  case AttributeType::text:
   [[fallthrough]];
  case AttributeType::name:
   [[fallthrough]];
  case AttributeType::title_name:
   [[fallthrough]];
  case AttributeType::class_name: {
   value_ = dynamic_cast<IString*>(new String(input_value));
   result = true;
  }break;
   //!@ Color
  case AttributeType::bkcolor: {
   DWORD dwColor = wcstoul(input_value.wstr(), nullptr, 16);
   Vec4 vRgba;
   Parser::parser_rgba(dwColor, vRgba);
   value_ = vRgba;
   result = true;
  }break;
   //!@ bool
  case AttributeType::floating:
   [[fallthrough]];
  case AttributeType::center:
   [[fallthrough]];
  case AttributeType::bold:
   [[fallthrough]];
  case AttributeType::visible:
   [[fallthrough]];
  case AttributeType::maximize: {
   value_ = input_value.compare("true", true);
   result = true;
  }break;
   //!@ Resource
  case AttributeType::font:
   [[fallthrough]];
  case AttributeType::logo: {
   value_ = wcstoul(input_value.wstr(), nullptr, 10);
   result = true;
  }break;
   //!@ Vec2
  case AttributeType::position:
   [[fallthrough]];
  case AttributeType::maxsize:
   [[fallthrough]];
  case AttributeType::minsize:
   [[fallthrough]];
  case AttributeType::size: {
   switch (control_type_) {
   case ControlType::Font: {
    if (input_value.empty())
     break;
    value_ = wcstof(input_value.wstr(), nullptr);
    result = true;
   }break;
   default: {
    if (input_value.empty())
     break;
    std::vector<std::wstring> parser = shared::Win::StringSpiltW(input_value.wstr(), L",");
    if (parser.size() != 2)
     break;
    value_ = Vec2(wcstof(parser[0].c_str(), nullptr), wcstof(parser[1].c_str(), nullptr));
    result = true;
   }break;
   }

  }break;
   //!@ Vec4
  case AttributeType::sizebox: {
   if (input_value.empty())
    break;
   std::vector<std::wstring> parser = shared::Win::StringSpiltW(input_value.wstr(), L",");
   if (parser.size() != 4)
    break;
   value_ = Vec4(wcstof(parser[0].c_str(), nullptr), wcstof(parser[1].c_str(), nullptr), wcstof(parser[2].c_str(), nullptr), wcstof(parser[3].c_str(), nullptr));
   result = true;
  }break;
  default:
   break;
  }
  return result;
 }

 void Attribute::Release() const {
  delete this;
 }
 const AttributeType& Attribute::GetType() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return attribute_type_;
 }
 bool Attribute::GetBool() const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 Vec2 Attribute::GetVec2() const {
  Vec2 result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 Vec4 Attribute::GetVec4() const {
  Vec4 result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 TypeIdentify Attribute::GetIdentify() const {
  TypeIdentify result = 0;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 TypePixels Attribute::GetFloat() const {
  TypePixels result = 0.0f;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 const ICache* Attribute::GetCache() const {
  ICache* result = nullptr;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 const IString* Attribute::GetString() const {
  IString* result = nullptr;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *this >> (result);
  return result;
 }
 const IControl* Attribute::ResourceRef() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return resource_ref_;
 }
 void Attribute::ResourceRef(const IControl* res) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  resource_ref_ = const_cast<IControl*>(res);
 }
}///namespace local