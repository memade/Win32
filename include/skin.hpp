#if !defined(__99F309C9_498C_41BA_AB96_57383F5B8A28__)
#define __99F309C9_498C_41BA_AB96_57383F5B8A28__

#include <dllinterface.hpp>

namespace skin {
 using TypePixels = float;
 using TypeArgb = unsigned long;
 using TypeIdentify = unsigned long;
 using TypeControl = unsigned long;
 using TypeAttribute = unsigned long;
 using TypeLayout = unsigned int;

 class ICache {
 public:
  virtual void Release() const = 0;
  virtual const char* Buffer() const = 0;
  virtual const size_t& Size() const = 0;
 };

 class IString {
 public:
  virtual void Release() const = 0;
  virtual const char* str() const = 0;
  virtual const wchar_t* wstr() const = 0;
  virtual const char8_t* u8() const = 0;
  virtual bool empty() const = 0;
  virtual bool compare(const wchar_t*, const bool& i = false) const = 0;
  virtual bool compare(const char*, const bool& i = false) const = 0;
  virtual bool compare(const IString*, const bool& i = false) const = 0;
 };

 struct Vec2 {
  TypePixels x, y;
  constexpr Vec2() : x(0.0f), y(0.0f) { }
  constexpr Vec2(const TypePixels& _x, const TypePixels& _y) : x(_x), y(_y) { }
  TypePixels& operator[] (size_t idx) { assert(idx == 0 || idx == 1); return ((TypePixels*)(void*)(char*)this)[idx]; }
  TypePixels  operator[] (size_t idx) const { assert(idx == 0 || idx == 1); return ((const TypePixels*)(const void*)(const char*)this)[idx]; }
  void operator=(const Vec2& obj) { x = obj.x; y = obj.y; }
  bool empty() const { return x == 0 && y == 0; }
  const TypePixels& GetX() const { return x; }
  const TypePixels& GetY() const { return y; }
  const TypePixels& GetCX() const { return x; }
  const TypePixels& GetCY() const { return y; }
  const TypePixels& GetWidth() const { return x; }
  const TypePixels& GetHeight() const { return y; }
  void SetX(const TypePixels& v_) { x = v_; }
  void SetY(const TypePixels& v_) { y = v_; }
  void SetCX(const TypePixels& v_) { x = v_; }
  void SetCY(const TypePixels& v_) { y = v_; }
  void SetWidth(const TypePixels& v_) { x = v_; }
  void SetHeight(const TypePixels& v_) { y = v_; }
 };

 struct Vec4 {
  TypePixels x, y, z, w;
  constexpr Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
  constexpr Vec4(const TypePixels& _x, const TypePixels& _y, const TypePixels& _z, const TypePixels& _w) : x(_x), y(_y), z(_z), w(_w) { }
  void operator=(const Vec4& obj) { x = obj.x; y = obj.y; z = obj.z; w = obj.w; }
  bool empty() const { return x == 0 && y == 0 && z == 0 && w == 0; }
  void SetX(const TypePixels& v_) { x = v_; }
  void SetY(const TypePixels& v_) { y = v_; }
  void SetCX(const TypePixels& v_) { z = v_; }
  void SetCY(const TypePixels& v_) { w = v_; }
  void SetWidth(const TypePixels& v_) { z = v_; }
  void SetHeight(const TypePixels& v_) { w = v_; }
  void SetPosition(const Vec2& pos) { x = pos.x; y = pos.y; }
  void SetSize(const Vec2& size) { z = size.x; w = size.y; }
  const TypePixels& GetX() const { return x; }
  const TypePixels& GetY() const { return y; }
  const TypePixels& GetCX() const { return z; }
  const TypePixels& GetCY() const { return w; }
  const TypePixels& GetWidth() const { return z; }
  const TypePixels& GetHeight() const { return w; }
  Vec2 GetSize() const { return Vec2(z, w); }
  Vec2 GetPosition() const { return Vec2(x, y); }
  const TypePixels& GetA() const { return x; }
  const TypePixels& GetR() const { return y; }
  const TypePixels& GetG() const { return z; }
  const TypePixels& GetB() const { return w; }
  DWORD GetARGB() const { return SK_COLOR_ARGB((int)(x * 0xFF), (int)(y * x * 0xFF), (int)(z * x * 0xFF), (int)(w * x * 0xFF)); }
 };

 enum LayoutType : TypeLayout {
  Floating = 0x0, //!@ Floating
  Vertical = 0x1,//!@ Vertical
  Horizontal = 0x2,//!@ Horizontal
 };

 typedef enum class NodeType : TypeControl {
  Unknown = 0x0000,
  Window = 0x0100,
  Frame = 0x0200,
  Control = 0x0300,
  Container = 0x0400,
  VerticalLayout = 0x0500,
  HorizontalLayout = 0x0600,
  TileLayout = 0x0700,
  TabLayout = 0x0800,
  Combo = 0x0900,
  Label = 0x0A00,
  Button = 0x0B00,
  Option = 0x0C00,
  Text = 0x0D00,
  Progress = 0x0E00,
  Slider = 0x0F00,
  Edit = 0x1000,
  ScrollBar = 0x2000,
  List = 0x3000,
  ListHeader = 0x4000,
  ListHeaderItem = 0x5000,
  ListLabelElement = 0x6000,
  ListTextElement = 0x7000,
  ListContainerElement = 0x8000,
  RichEdit = 0x9000,
  TreeView = 0xA000,
  TreeNode = 0xB000,
  Resources = 0xC000,
  Font = Resources | 0x100,
  Image = Resources | 0x200,
  UserData = Resources | 0x300,
 }ControlType, ResourcesType;

 enum class AttributeType : TypeAttribute {
  unknown = 0x0,
  identify = 0x1,
  position = 0x2,
  sizebox = 0x3,
  caption = 0x4,
  size = 0x5,
  borderround = 0x6,
  roundcorner = 0x7,
  minsize = 0x8,
  maxsize = 0x9,
  bkcolor = 0xA,
  bordercolor = 0xB,
  bordersize = 0xC,
  visible = 0xD,
  rounding = 0xE,
  alpha = 0xF,
  floating = 0x10,
  pixels = 0x11,
  bold = 0x12,
  font = 0x13,
  class_name = 0x14,
  fontfile = 0x15,
  bkimage = 0x16,
  logo = 0x17,
  name = 0x18,
  text = 0x19,
  layout = 0x1A,
  maximize = 0x1B,
  file = 0x1D,
  title_name = 0x1E,
  center = 0x1F,
  width = 0x20,
  height = 0x21,
 };

 class IAttribute {
 public:
  virtual void Release() const = 0;
  virtual const AttributeType& GetType() const = 0;
  virtual bool GetBool() const = 0;
  virtual Vec2 GetVec2() const = 0;
  virtual Vec4 GetVec4() const = 0;
  virtual TypeIdentify GetIdentify() const = 0;
  virtual TypePixels GetFloat() const = 0;
  virtual const ICache* GetCache() const = 0;
  virtual const IString* GetString() const = 0;
  virtual const class IControl* ResourceRef() const = 0;
  virtual void ResourceRef(const class IControl*) = 0;
 };

 class IAttributes {
 public:
  virtual const ControlType& GetControlType() const = 0;
  virtual const IAttribute* GetAttribute(const AttributeType&) const = 0;
  virtual void Release() const = 0;
  virtual void Push(const IAttribute*) = 0;
  virtual bool Empty() const = 0;
  virtual unsigned long Total() const = 0;
  virtual const IAttribute* Next(const unsigned long&) const = 0;
  virtual const IString* GetTitleName(const AttributeType& attribute_type = AttributeType::title_name) const = 0;
  virtual Vec2 GetSize(const AttributeType& attribute_type = AttributeType::size) const = 0;
  virtual Vec4 GetBkColor(const AttributeType& attribute_type = AttributeType::bkcolor) const = 0;
  virtual const ICache* GetCache(const AttributeType& attribute_type = AttributeType::file) const = 0;
  virtual void Iterator(const std::function<void(const AttributeType&, IAttribute*, bool&)>&) const = 0;
 };

 class IControl {
 protected:
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
  IControl* bind_control_ = nullptr;
  std::uintptr_t initial_pointer_ = 0;
  IControl* parent_ = nullptr;
  TypeIdentify identify_ = 0;
  ControlType control_type_ = ControlType::Unknown;
  LayoutType layout_type_ = LayoutType::Vertical;
  IAttributes* attribute_s_ = nullptr;
  Vec4 layout_data_;
 public:
  virtual void Release() const = 0;
  virtual const LayoutType& GetLayoutType() const { std::lock_guard<std::mutex>(*m_Mutex); return layout_type_; }
  virtual const TypeIdentify& Identify() const { std::lock_guard<std::mutex>(*m_Mutex); return identify_; }
  virtual void Identify(const TypeIdentify& id) { std::lock_guard<std::mutex>(*m_Mutex); identify_ = id; }
  virtual const ControlType& GetControlType() const { std::lock_guard<std::mutex>(*m_Mutex); return control_type_; }
  virtual void Parent(IControl* parent) { std::lock_guard<std::mutex>(*m_Mutex); parent_ = parent; }
  virtual IControl* Parent() const { std::lock_guard<std::mutex>(*m_Mutex); return parent_; }
  virtual const std::uintptr_t& GetInitialPointer() const { std::lock_guard<std::mutex>(*m_Mutex); return initial_pointer_; }
  virtual void Bind(IControl* ctrl) { std::lock_guard<std::mutex>(*m_Mutex); bind_control_ = ctrl; }
  virtual IControl* Bind() const { std::lock_guard<std::mutex>(*m_Mutex); return bind_control_; }
  virtual const Vec4* GetLayoutData() const { std::lock_guard<std::mutex>(*m_Mutex); return &layout_data_; }
  virtual void SetLayoutData(const Vec4& layout) { std::lock_guard<std::mutex>(*m_Mutex);  layout_data_ = layout; }
 };

 typedef class IControls {
 public:
  virtual void Release() const = 0;
  virtual bool Empty() const = 0;
  virtual void Push(IControl*) = 0;
  virtual unsigned long Total() const = 0;
  virtual IControl* Next(const unsigned long&) const = 0;
  virtual void Iterator(const std::function<void(IControl*, bool&)>&) const = 0;
  virtual void LayoutIterator(const std::function<void(IControl*, bool&)>&) const = 0;
  virtual unsigned long LayoutCount() const = 0;
 }IControlChilds;

 typedef class IControlSK : public IControl {
 public:
  virtual void PushChild(IControl*) = 0;
  virtual const IAttributes* GetAttributes() const { std::lock_guard<std::mutex>(*m_Mutex); return attribute_s_; }
  virtual Vec2 GetSize() const = 0;
  virtual Vec2 GetPosition() const = 0;
  virtual bool IsFloating() const = 0;
 }IResource;

 class IControlUI : public IControl {
 protected:
  // Whether to start | begin
  bool rendering_flags_ = false;
 public:
  virtual void OnLayout() = 0;
  virtual void OnRenderBegin() = 0;
  virtual void OnRenderEnd() = 0;
  virtual void SetAttributes(const IAttributes* assign) { \
   std::lock_guard<std::mutex>(*m_Mutex); attribute_s_ = const_cast<IAttributes*>(assign); }
  virtual const bool& RenderingFlags() const { std::lock_guard<std::mutex>(*m_Mutex); return rendering_flags_; }
  virtual void RenderingFlags(const bool& flags) { std::lock_guard<std::mutex>(*m_Mutex); rendering_flags_ = flags; }
  //GetSize actually is GetClientSize.
  virtual Vec2 GetSize() const { std::lock_guard<std::mutex>(*m_Mutex); return Vec2(); }
 };

 class IResources {
 public:
  virtual void Release() const = 0;
  virtual void Push(IResource*) = 0;
  virtual void Pop(const TypeIdentify&) = 0;
  virtual void Clear() = 0;
  virtual IResource* Search(const TypeIdentify&) const = 0;
 };

 class ISkinUI {
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual bool SkinCreate(const char*) = 0;
  virtual void SkinDestroy() = 0;
  virtual void Release() const = 0;
  virtual class ISkin* SkinGet() const = 0;
  virtual shared::IUIConfig* UIConfigGet() const = 0;
  virtual IControlUI* CreateControl(const ControlType&) = 0;
 };

 class ISkin : public shared::InterfaceDll<ISkin> {
 public:
  virtual void Release() const = 0;
  virtual bool Perform() = 0;
  virtual void Render() = 0;
  virtual void Layout() = 0;
  virtual bool Ready() const = 0;
  virtual void SetUIModule(ISkinUI*) = 0;
  virtual bool SkinConfigure(const char*) = 0;
 };

}///namespace skin


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 01:16:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__99F309C9_498C_41BA_AB96_57383F5B8A28__