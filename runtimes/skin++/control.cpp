#include "stdafx.h"

namespace local {
 /////////////////////////////////////////////////////////////////////////////////////
 NodeRender::NodeRender(const TypeIdentifyType& identify, const IResources* res, const ControlType& control_type)
  : control_type_(control_type)
  , resources_(res)
  , identify_(identify) {

 }
 NodeRender::~NodeRender() {

 }
 //-----------------------------------------------------------------------------------------------------------
 //-----------------------------------------------------------------------------------------------------------
 bool NodeRender::operator<<(const std::array<Strings, 2>& input_attribute_node) {
  bool result = false;
  do {
   Strings Name = input_attribute_node[0];
   Strings Value = input_attribute_node[1];
   if (Name.empty())
    break;

   const bool compare_i = true;


   if (Name.compare("size", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     size_.cx = static_cast<decltype(size_.cx)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     size_.cy = static_cast<decltype(size_.cy)>(::atol(parser[1].c_str()));
   }
   else if (Name.compare("sizebox", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     sizebox_.left = static_cast<decltype(sizebox_.left)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     sizebox_.top = static_cast<decltype(sizebox_.top)>(::atol(parser[1].c_str()));
    if (parser.size() > 2)
     sizebox_.right = static_cast<decltype(sizebox_.right)>(::atol(parser[2].c_str()));
    if (parser.size() > 3)
     sizebox_.bottom = static_cast<decltype(sizebox_.bottom)>(::atol(parser[3].c_str()));
   }
   else if (Name.compare("caption", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     caption_.left = static_cast<decltype(caption_.left)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     caption_.top = static_cast<decltype(caption_.top)>(::atol(parser[1].c_str()));
    if (parser.size() > 2)
     caption_.right = static_cast<decltype(caption_.right)>(::atol(parser[2].c_str()));
    if (parser.size() > 3)
     caption_.bottom = static_cast<decltype(caption_.bottom)>(::atol(parser[3].c_str()));
   }
   else if (Name.compare("roundcorner", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     roundcorner_.cx = static_cast<decltype(roundcorner_.cx)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     roundcorner_.cy = static_cast<decltype(roundcorner_.cy)>(::atol(parser[1].c_str()));
   }
   else if (Name.compare("mininfo", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     mininfo_.cx = static_cast<decltype(mininfo_.cx)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     mininfo_.cy = static_cast<decltype(mininfo_.cy)>(::atol(parser[1].c_str()));
   }
   else if (Name.compare("maxinfo", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     maxinfo_.cx = static_cast<decltype(maxinfo_.cx)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     maxinfo_.cy = static_cast<decltype(maxinfo_.cy)>(::atol(parser[1].c_str()));
   }
   else if (Name.compare("bkcolor", compare_i)) {
    bkcolor_ = strtoul(Value.str(), nullptr, 16);
   }
   else if (Name.compare("bordercolor", compare_i)) {
    bordercolor_ = strtoul(Value.str(), nullptr, 16);
   }
   else if (Name.compare("bordersize", compare_i)) {
    bordersize_ = ::atoi(Value.str());
   }
   else if (Name.compare("borderround", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     borderround_.cx = static_cast<decltype(borderround_.cx)>(::atol(parser[0].c_str()));
    if (parser.size() > 1)
     borderround_.cy = static_cast<decltype(borderround_.cy)>(::atol(parser[1].c_str()));
   }
   else if (Name.compare("width", compare_i)) {
    width_ = ::atoi(Value.str());
   }
   else if (Name.compare("height", compare_i)) {
    height_ = ::atoi(Value.str());
   }
   else if (Name.compare("bkimage", compare_i)) {
    std::string pathname = resources_->Path();
    pathname = shared::Win::PathFixedA(pathname + "\\" + Value.toString());
    bkimage_cache_ = shared::Win::File::ReadCXX(pathname);
    bkimage_pathname_ << pathname;
   }
   else if (Name.compare("name", compare_i)) {
    name_ = Value;
   }
   else if (Name.compare("visible", compare_i)) {
    visible_ = !::stricmp("true", Value.str()) ? true : false;
   }
   else if (Name.compare("text", compare_i)) {
    text_ = Value;
   }
   else if (Name.compare("logo", compare_i)) {
    std::string pathname = resources_->Path();
    pathname = shared::Win::PathFixedA(pathname + "\\" + Value.toString());
    logo_cache_ = shared::Win::File::ReadCXX(pathname);
    logo_pathname_ << pathname;
    logo_icon_ = shared::Win::Window::LoadMemIcon(logo_cache_);
   }
   else if (Name.compare("rounding", compare_i)) {
    rounding_ = ::atof(Value.str());
   }
   else if (Name.compare("alpha", compare_i)) {
    alpha_ = ::atof(Value.str());
   }
   else if (Name.compare("floating", compare_i)) {
    floating_ = !::stricmp("true", Value.str()) ? true : false;
   }
   else if (Name.compare("pos", compare_i)) {
    std::vector<std::string> parser = shared::Win::StringSpiltA(Value.toString(), ",");
    if (parser.size() > 0)
     pos_.x = static_cast<decltype(pos_.x)>(::atoi(parser[0].c_str()));
    if (parser.size() > 1)
     pos_.y = static_cast<decltype(pos_.y)>(::atoi(parser[1].c_str()));
   }
   else if (Name.compare("identify", compare_i)) {
    identify_ = ::atol(Value.str());
   }
   else if (Name.compare("fontfile_pathname", compare_i)) {
    std::string pathname = resources_->Path();
    pathname = shared::Win::PathFixedA(pathname + "\\" + Value.toString());
    font_cache_ = shared::Win::File::ReadCXX(pathname);
    fontfile_pathname_ << pathname;
   }
   else if (Name.compare("pixels", compare_i)) {
    pixels_ = ::atof(Value.str());
   }
   else if (Name.compare("bold", compare_i)) {
    identify_ = ::atol(Value.str());
   }
   else if (Name.compare("font", compare_i)) {
    font_ = ::atol(Value.str());
   }
   else if (Name.compare("class_name", compare_i)) {
    class_name_ = Value;
   }
   else if (Name.compare("layout", compare_i)) {
    if (Value.compare("vl", compare_i)) {
     layout_ = LayoutType::Vertical;
    }
    else if (Value.compare("hl", compare_i)) {
     layout_ = LayoutType::Horizontal;
    }
    else if (Value.compare("fl", compare_i)) {
     layout_ = LayoutType::Floating;
    }
    else {
     TypeLayoutType v = atoi(Value.str());
     switch (LayoutType(v)) {
     case LayoutType::Vertical:
      layout_ = LayoutType(v);
      break;
     case LayoutType::Horizontal:
      layout_ = LayoutType(v);
      break;
     case LayoutType::Floating:
      layout_ = LayoutType(v);
      break;
     default:
      break;
     }
    }
   }
   else {
    break;
   }
   result = true;
  } while (0);
  return result;
 }

 const IStrings* NodeRender::ClassName() const {
  return dynamic_cast<const IStrings*>(&class_name_);
 }
 TypeIdentifyType NodeRender::Identify() const {
  return identify_;
 }
 const IResources* NodeRender::GetRes() const {
  return resources_;
 }
 ControlType NodeRender::GetType() const {
  return control_type_;
 }
 void NodeRender::Handle(void* handle) {
  handle_ = handle;
 }
 void* NodeRender::Handle() const {
  return handle_;
 }
 void NodeRender::Route(void* ptr) {
  route_ = ptr;
 }
 void* NodeRender::Route() const {
  return route_;
 }
 const tfSkinNodes& NodeRender::Childs() const {
  return child_node_s_;
 }
 void NodeRender::Parent(INodeRender* node) {
  parent_ = node;
 }
 INodeRender* NodeRender::Parent() const {
  return parent_;
 }
 INodeRender* NodeRender::BeginChild() const {
  return child_node_s_.empty() ? nullptr : child_node_s_[0];
 }
 INodeRender* NodeRender::EndChild() const {
  return child_node_s_.empty() ? nullptr : *std::prev(child_node_s_.end());
 }
 size_t NodeRender::TotalChild() const {
  return child_node_s_.size();
 }
 INodeRender* NodeRender::NextChild(const size_t& index) const {
  INodeRender* result = nullptr;
  do {
   if (index >= child_node_s_.size())
    break;
   result = child_node_s_[index];
  } while (0);
  return result;
 }
 LayoutType NodeRender::Layout() const {
  return layout_;
 }
 void NodeRender::Push(INodeRender* node) {
  if (node)
   child_node_s_.emplace_back(node);
 }
 std::uintptr_t NodeRender::NodeKey() const {
  return node_key_;
 }
 void NodeRender::NodeKey(const std::uintptr_t& key) {
  node_key_ = key;
 }
 PSIZE NodeRender::Size() const {
  return const_cast<const PSIZE>(&size_);
 }
 PRECT NodeRender::SizeBox() const {
  return const_cast<const PRECT>(&sizebox_);
 }
 PRECT NodeRender::Caption() const {
  return const_cast<const PRECT>(&caption_);
 }
 PSIZE NodeRender::RoundCorner() const {
  return const_cast<const PSIZE>(&roundcorner_);
 }
 PSIZE NodeRender::Mininfo() const {
  return const_cast<const PSIZE>(&mininfo_);
 }
 PSIZE NodeRender::Maxinfo() const {
  return const_cast<const PSIZE>(&maxinfo_);
 }
 PSIZE NodeRender::BorderRound() const {
  return const_cast<const PSIZE>(&borderround_);
 }
 PPOINT NodeRender::Pos() const {
  return const_cast<const PPOINT>(&pos_);
 }
 TypeArgbType NodeRender::BkColor() const {
  return bkcolor_;
 }
 TypeArgbType NodeRender::BorderColor() const {
  return bordercolor_;
 }
 ARGB NodeRender::BkColorArgb() const {
  ARGB result;
  std::uint8_t a;
  std::uint8_t r;
  std::uint8_t g;
  std::uint8_t b;
  Parser::parser_rgba(bkcolor_, a, r, g, b);
  result.a = a / 255.0;
  result.r = r / 255.0;
  result.g = g / 255.0;
  result.b = b / 255.0;
  return result;
 }
 ARGB NodeRender::BorderColorArgb() const {
  ARGB result;
  std::uint8_t a;
  std::uint8_t r;
  std::uint8_t g;
  std::uint8_t b;
  Parser::parser_rgba(bordercolor_, a, r, g, b);
  result.a = a / 255.0;
  result.r = r / 255.0;
  result.g = g / 255.0;
  result.b = b / 255.0;
  return result;
 }
 int NodeRender::BorderSize() const {
  return bordersize_;
 }
 int NodeRender::Width() const {
  return width_;
 }
 int NodeRender::Height() const {
  return height_;
 }
 const IStrings* NodeRender::Name() const {
  return dynamic_cast<const IStrings*>(&name_);
 }
 const IStrings* NodeRender::BkImage() const {
  return dynamic_cast<const IStrings*>(&bkimage_pathname_);
 }
 void NodeRender::BkImage(const tfCacheCb& cache_cb) const {
  cache_cb(bkimage_cache_.c_str(), bkimage_cache_.size());
 }
 const IStrings* NodeRender::Text() const {
  return dynamic_cast<const IStrings*>(&text_);
 }
 bool NodeRender::Visible() const {
  return visible_;
 }
 const IStrings* NodeRender::LogoPathname() const {
  return dynamic_cast<const IStrings*>(&logo_pathname_);
 }
 void NodeRender::Logo(const tfCacheCb& cache_cb) const {
  cache_cb(logo_cache_.c_str(), logo_cache_.size());
 }
 HICON NodeRender::LogoIcon() const {
  return logo_icon_;
 }
 float NodeRender::Rounding() const {
  return rounding_;
 }
 bool NodeRender::Floating() const {
  return floating_;
 }
 float NodeRender::Alpha() const {
  return alpha_;
 }
 const IStrings* NodeRender::FontFilePathname() const {
  return dynamic_cast<const IStrings*>(&fontfile_pathname_);
 }
 float NodeRender::Pixels() const {
  return pixels_;
 }
 bool NodeRender::Bold() const {
  return bold_;
 }
 void NodeRender::FontCache(const tfCacheCb& cache_cb) const {
  cache_cb(font_cache_.c_str(), font_cache_.size());
 }
 TypeIdentifyType NodeRender::FontIdentify() const {
  return font_;
 }
 void NodeRender::FontHandle(void* font_handle) {
  font_handle_ = font_handle;
 }
 void* NodeRender::FontHandle() const {
  return font_handle_;
 }
 bool NodeRender::FlagInitialization() const {
  return flag_initialization_;
 }
 void NodeRender::FlagInitialization(const bool& flag) {
  flag_initialization_ = flag;
 }
 void NodeRender::DefaultConfigCover() {
  return;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Window::Window(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Window) {

 }
 Window::~Window() {

 }
 void Window::Release() const {
  delete this;
 }
 void Window::DefaultConfigCover() {
  layout_ = LayoutType::Vertical;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Frame) {

 }
 Frame::~Frame() {

 }
 void Frame::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Control::Control(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Control) {

 }
 Control::~Control() {

 }
 void Control::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Container::Container(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Container) {

 }
 Container::~Container() {

 }
 void Container::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 VerticalLayout::VerticalLayout(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::VerticalLayout) {

 }
 VerticalLayout::~VerticalLayout() {

 }
 void VerticalLayout::Release() const {
  delete this;
 }
 void VerticalLayout::DefaultConfigCover() {
  layout_ = LayoutType::Vertical;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 HorizontalLayout::HorizontalLayout(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::HorizontalLayout) {

 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }
 void HorizontalLayout::DefaultConfigCover() {
  layout_ = LayoutType::Horizontal;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 TabLayout::TabLayout(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::TabLayout) {

 }
 TabLayout::~TabLayout() {

 }
 void TabLayout::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Button::Button(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Button) {

 }
 Button::~Button() {

 }
 void Button::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Label::Label(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Label) {

 }
 Label::~Label() {

 }
 void Label::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 Font::Font(const TypeIdentifyType& identify, const IResources* res) : NodeRender(identify, res, ControlType::Font) {

 }
 Font::~Font() {

 }
 void Font::Release() const {
  delete this;
 }
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////
}///namespace local