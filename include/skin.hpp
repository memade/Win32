#if !defined(__99F309C9_498C_41BA_AB96_57383F5B8A28__)
#define __99F309C9_498C_41BA_AB96_57383F5B8A28__

#include <dllinterface.hpp>

namespace skin {
 using TypeIdentifyType = unsigned int;
 using TypeFrameType = unsigned long;
 using TypeControlType = unsigned long;
 using TypeArgbType = unsigned long;
 using TypeResourcesType = unsigned long;
 using TypeLayoutType = unsigned int;
 using TypeCacheType = std::string;
 using tfCacheCb = std::function<void(const char*, const size_t&)>;
 using tfSkinNodes = std::vector<class INodeRender*>;
 using tfFontNodes = std::map<TypeIdentifyType, class INodeRender*>;
 using tfResNodes = std::map<std::string, class IResources*>;
 using tfNodeCb = std::function<void(class INodeRender*)>;
 using tfNodeEndCb = tfNodeCb;
 using tfNodeBeginCb = tfNodeCb;
 using tfNodeCreateCb = tfNodeCb;
 using tfNodeDestroyCb = tfNodeCb;

 class IStrings {
 public:
  virtual const char* str() const = 0;
  virtual const wchar_t* wstr() const = 0;
  virtual const char8_t* u8() const = 0;
  virtual bool empty() const = 0;
  virtual bool compare(const wchar_t*, const bool& i = false) const = 0;
  virtual bool compare(const char*, const bool& i = false) const = 0;
  virtual bool compare(const IStrings*, const bool& i = false) const = 0;
 };

 enum LayoutType : TypeLayoutType {
  Floating = 0x0, //!@ Floating
  Vertical = 0x1,//!@ Vertical
  Horizontal = 0x2,//!@ Horizontal
 };

 typedef struct _ARGB {
  float a;
  float r;
  float g;
  float b;

  _ARGB() { ::memset(this, 0x00, sizeof(*this)); }
  void operator=(const _ARGB& obj) { ::memcpy(this, &obj, sizeof(*this)); }
 }ARGB, * PARGB;

 enum class ResourcesType : TypeResourcesType {
  UNKNOWN = 0x0000,
  CONFIG = 0x1000,
  CONFIG_XML = CONFIG | 0x100,
  CONFIG_JSON = CONFIG | 0x200,
  IMAGE = 0x2000,
  IMAGE_ICON_ = IMAGE | 0x100,
  IMAGE_PNG = IMAGE | 0x200,
  IMAGE_JPG = IMAGE | 0x300,
  IMAGE_GIF = IMAGE | 0x400,
  DATA = 0x3000,
  DATA_BIN = DATA | 0x100,
  DATA_ZIP = DATA | 0x200,
  FONT = 0x4000,
  FONT_TTF = FONT | 0x100,
  FONT_TTC = FONT | 0x200,
 };

 enum class ControlType : TypeControlType {
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
  Font = 0xC000,
 };

 class IResources {
 public:
  virtual void Release() const = 0;
  virtual const ResourcesType& Type() const = 0;
  virtual const char* Path() const = 0;
  virtual void Path(const char*) = 0;
  virtual const char* Pathname() const = 0;
  virtual void Pathname(const char*) = 0;
  virtual void Cache(const char*, const size_t&) = 0;
  virtual void Cache(const tfCacheCb&) const = 0;
  virtual bool Load() = 0;
 };

 class INodeRender {
 public:
  virtual void DefaultConfigCover() {}
 public:
  virtual TypeIdentifyType Identify() const { return TypeIdentifyType(0); }
  virtual ControlType GetType() const { return ControlType::Unknown; }
  virtual void Release() const {}
  virtual void Parent(INodeRender*) {}
  virtual INodeRender* Parent() const { return nullptr; }
  virtual INodeRender* BeginChild() const { return nullptr; }
  virtual INodeRender* EndChild() const { return nullptr; }
  virtual INodeRender* NextChild(const size_t&) const { return nullptr; }
  virtual size_t TotalChild() const { return 0; }
  virtual void Push(INodeRender*) {}
  virtual std::uintptr_t NodeKey() const { return 0; }
  virtual void NodeKey(const std::uintptr_t&) {}
  virtual const IResources* GetRes() const { return nullptr; }
  virtual void Handle(void*) {}
  virtual void* Handle() const { return nullptr; }
  virtual PSIZE Size() const { return nullptr; }
  virtual PRECT SizeBox() const { return nullptr; }
  virtual PRECT Caption() const { return nullptr; }
  virtual PSIZE RoundCorner() const { return nullptr; }
  virtual PSIZE Mininfo() const { return nullptr; }
  virtual PSIZE Maxinfo() const { return nullptr; }
  virtual PSIZE BorderRound() const { return nullptr; }
  virtual PPOINT Pos() const { return nullptr; }
  virtual TypeArgbType BkColor() const { return 0; }
  virtual TypeArgbType BorderColor() const { return 0; }
  virtual ARGB BkColorArgb() const { return ARGB(); }
  virtual ARGB BorderColorArgb() const { return ARGB(); }
  virtual int BorderSize() const { return 0; }
  virtual int Width() const { return 0; }
  virtual int Height() const { return 0; }
  virtual const IStrings* Name() const { return nullptr; }
  virtual const IStrings* BkImage() const { return nullptr; }
  virtual void BkImage(const tfCacheCb&) const {}
  virtual const IStrings* Text() const { return nullptr; }
  virtual bool Visible() const { return false; }
  virtual const IStrings* LogoPathname() const { return nullptr; }
  virtual void Logo(const tfCacheCb&) const {}
  virtual HICON LogoIcon() const { return nullptr; }
  virtual float Rounding() const { return 0.0f; }
  virtual bool Floating() const { return false; }
  virtual float Alpha() const { return 0.0f; }
  virtual const IStrings* FontFilePathname() const { return nullptr; }
  virtual float Pixels() const { return 0.0f; }
  virtual bool Bold() const { return false; }
  virtual void FontCache(const tfCacheCb&) const {}
  virtual TypeIdentifyType FontIdentify() const { return 0; }
  virtual void FontHandle(void*) {}
  virtual void* FontHandle() const { return nullptr; }
  virtual void Route(void*) {}
  virtual void* Route() const { return nullptr; }
  virtual bool FlagInitialization() const { return false; }
  virtual void FlagInitialization(const bool&) {}
  virtual const IStrings* ClassName() const { return nullptr; }
  virtual LayoutType Layout() const { return LayoutType::Floating; }
 };

 class IWindow : public INodeRender {
 public:

 };

 class IControl : public INodeRender {
 public:

 };

 class IContainer : public IControl {
 public:

 };

 class IVerticalLayout : public IControl {
 public:

 };

 class IHorizontalLayout : public IControl {
 public:

 };

 class ITileLayout : public IContainer {
 public:

 };

 class ITabLayout : public IContainer {
 public:

 };

 class ICombo : public IContainer {
 public:

 };

 class ILabel : public IControl {
 public:

 };

 class IButton : public ILabel {
 public:

 };

 class IOption : public IButton {
 public:

 };

 class IText : public ILabel {
 public:

 };

 class IProgress : public ILabel {
 public:

 };

 class ISlider : public IProgress {
 public:

 };

 class IEdit : public ILabel {
 public:

 };

 class IScrollBar : public IControl {
 public:

 };

 class IList : public IVerticalLayout {
 public:

 };

 class IListHeader : public IHorizontalLayout {
 public:

 };

 class IListHeaderItem : public IControl {
 public:

 };

 class IListLabelElement : public IControl {
 public:

 };

 class IListTextElement : public IListLabelElement {
 public:

 };

 class IListContainerElement : public IContainer {
 public:

 };

 class IRichEdit : public IContainer {
 public:

 };

 class ITreeView : public IList {
 public:

 };

 class ITreeNode : public IListContainerElement {
 public:

 };

 class ISkinUI {
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual void Center() const = 0;
  virtual bool Skin(const char*) = 0;
  virtual void Release() const = 0;
  virtual class ISkin* SkinGet() const = 0;
  virtual shared::IUIConfig* UIConfigGet() const = 0;
  virtual void Render() = 0;
  virtual bool CreateMainWindow() = 0;
 };

 class ISkin : public shared::InterfaceDll<ISkin> {
 public:
  virtual void Release() const = 0;
  virtual bool Perform() = 0;
  virtual void Render() = 0;
  virtual void From(ISkinUI*) = 0;
  virtual bool From(const char* skin_directory) = 0;
  virtual INodeRender* MainNode() const = 0;
  virtual void NodeIterator(const tfNodeBeginCb&, const tfNodeEndCb&) const = 0;
  virtual void FontNodeIterator(const tfNodeCb&) const = 0;
  virtual void RegisterNodeCreateCb(const tfNodeCreateCb&) = 0;
  virtual void RegisterNodeDestroyCb(const tfNodeCreateCb&) = 0;
  virtual void ParserArgb(const TypeArgbType&, std::uint8_t&, std::uint8_t&, std::uint8_t&, std::uint8_t&) const = 0;
 };




}///namespace skin


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 01:16:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__99F309C9_498C_41BA_AB96_57383F5B8A28__