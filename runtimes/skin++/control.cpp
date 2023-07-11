#include "stdafx.h"

namespace local {
 Controls::Controls() {

 }
 Controls::~Controls() {

 }
 void Controls::Release() const {
  delete this;
 }
 bool Controls::Empty() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return controls_.empty();
 }
 unsigned long Controls::Total() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return static_cast<unsigned long>(controls_.size());
 }
 IControl* Controls::Next(const unsigned long& idx) const {
  IControl* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  if (idx < controls_.size())
   result = controls_[idx];
  return result;
 }
 void Controls::Push(IControl* ctrl) {
  std::lock_guard<std::mutex>(*m_Mutex);
  controls_.emplace_back(ctrl);
 }
 void Controls::LayoutIterator(const std::function<void(IControl*, bool&)>& iterator_cb) const {
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto& it : controls_) {  
   auto pControlSK = dynamic_cast<IControlSK*>(it);
   if (!pControlSK)
    continue;
   if (pControlSK->IsFloating())
    continue;
   bool __break = false;
   iterator_cb(it, __break);
   if (__break)
    break;
  }
 }
 void Controls::Iterator(const std::function<void(IControl*, bool&)>& iterator_cb) const {
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto& it : controls_) {
   bool __break = false;
   iterator_cb(it, __break);
   if (__break)
    break;
  }
 }
 unsigned long Controls::LayoutCount() const {
  unsigned long result = 0;
  std::lock_guard<std::mutex>(*m_Mutex);
  for (auto& ctrl : controls_) {
   auto pControlSK = dynamic_cast<IControlSK*>(ctrl);
   do {
    if (!pControlSK)
     break;
    if (pControlSK->IsFloating())
     break;
    ++result;
   } while (0);
  }
  return result;
 }


 Control::Control(const ControlType& type, const String& skin_root_path)
  : skin_root_path_(skin_root_path) {
  control_type_ = type;
  child_s_ = dynamic_cast<IControls*>(new Controls());
  attribute_s_ = dynamic_cast<IAttributes*>(new Attributes(skin_root_path_, control_type_));
 }

 Control::~Control() {
  SK_RELEASE_PTR(attribute_s_);
  SK_RELEASE_PTR(child_s_);
 }
 const std::uintptr_t& Control::NodeKey() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return node_key_;
 }
 const std::uintptr_t& Control::ParentNodeKey() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return parent_node_key_;
 }
 void Control::NodeKey(const std::uintptr_t& key) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  node_key_ = key;
 }
 void Control::ParentNodeKey(const std::uintptr_t& key) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  parent_node_key_ = key;
 }
 const IControls* Control::Childs() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return child_s_;
 }
 void Control::PushChild(IControl* child) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (child)
   child_s_->Push(child);
 }
 void Control::SetAttribute(const AttributeType& type, const tfAttributeValueType& value) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  attribute_s_->Push(dynamic_cast<const IAttribute*>(new Attribute(control_type_, type, value)));
 }
 void Control::operator<<(const std::map<AttributeType, String>& input_attribute_s) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  *dynamic_cast<Attributes*>(attribute_s_) << input_attribute_s;
 }
 const IAttributes* Control::GetAttributes() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return attribute_s_;
 }
 Vec2 Control::GetSize() const {
  Vec2 result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!attribute_s_)
    break;
   auto att = attribute_s_->GetAttribute(AttributeType::size);
   if (!att)
    break;
   result = att->GetVec2();
  } while (0);
  return result;
 }
 Vec2 Control::GetPosition() const {
  Vec2 result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!attribute_s_)
    break;
   auto attPos = attribute_s_->GetAttribute(AttributeType::position);
   if (!attPos)
    break;
   bool floating = false;
   auto attFloating = attribute_s_->GetAttribute(AttributeType::floating);
   if (attFloating)
    floating = attFloating->GetBool();
   if (floating)
    result = attPos->GetVec2();
  } while (0);
   return result;
 }
 bool Control::IsFloating() const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!attribute_s_)
    break;
   auto att = attribute_s_->GetAttribute(AttributeType::floating);
   if (!att)
    break;
   result = att->GetBool();
  } while (0);
  return result;
 }

 Window::Window() : Control(ControlType::Window) {
  initial_pointer_ = reinterpret_cast<std::uintptr_t>(this);
  attribute_s_->Push(new Attribute(control_type_, AttributeType::size, Vec2(0, 0)));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::minsize, Vec2(0, 0)));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::maxsize, Vec2(0, 0)));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::alpha, 1.0f));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::visible, true));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::center, true));
 }
 Window::~Window() {

 }
 void Window::Release() const {
  delete this;
 }

 Frame::Frame() : Control(ControlType::Frame) {
  initial_pointer_ = reinterpret_cast<std::uintptr_t>(this);
  attribute_s_->Push(new Attribute(control_type_, AttributeType::visible, true));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::position, Vec2(0, 0)));
 }
 Frame::~Frame() {

 }
 void Frame::Release() const {
  delete this;
 }

 VerticalLayout::VerticalLayout() : Control(ControlType::VerticalLayout) {
  initial_pointer_ = reinterpret_cast<std::uintptr_t>(this);
  attribute_s_->Push(new Attribute(control_type_, AttributeType::visible, true));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::position, Vec2(0, 0)));
 }
 VerticalLayout::~VerticalLayout() {

 }
 void VerticalLayout::Release() const {
  delete this;
 }

 HorizontalLayout::HorizontalLayout() : Control(ControlType::HorizontalLayout) {
  layout_type_ = LayoutType::Horizontal;
  initial_pointer_ = reinterpret_cast<std::uintptr_t>(this);
  attribute_s_->Push(new Attribute(control_type_, AttributeType::visible, true));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::position, Vec2(0, 0)));
 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }

 Button::Button() : Control(ControlType::Button) {
  initial_pointer_ = reinterpret_cast<std::uintptr_t>(this);
  attribute_s_->Push(new Attribute(control_type_, AttributeType::visible, true));
  attribute_s_->Push(new Attribute(control_type_, AttributeType::position, Vec2(0, 0)));
 }
 Button::~Button() {

 }
 void Button::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Font::Font(const String& skin_root_path) : Control(ControlType::Font, skin_root_path) {

 }
 Font::~Font() {

 }
 void Font::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Image::Image(const String& skin_root_path) : Control(ControlType::Image, skin_root_path) {

 }
 Image::~Image() {

 }
 void Image::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 UserData::UserData(const String& skin_root_path) : Control(ControlType::UserData, skin_root_path) {

 }
 UserData::~UserData() {

 }
 void UserData::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////



}///namespace local