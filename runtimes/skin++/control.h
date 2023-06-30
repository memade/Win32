#if !defined(__828DAEA0_960A_4F3A_A9F0_0F14FF281869__)
#define __828DAEA0_960A_4F3A_A9F0_0F14FF281869__

namespace local {

 class NodeRender : public INodeRender {
 protected:
  NodeRender(const TypeIdentifyType&, const IResources*, const ControlType&);
  virtual ~NodeRender();
 public:
  virtual bool operator<<(const std::array<Strings, 2>&);
 protected:
  TypeIdentifyType Identify() const override;
  ControlType GetType() const override;
  void Parent(INodeRender*) override;
  INodeRender* Parent() const override;
  INodeRender* BeginChild() const override;
  INodeRender* EndChild() const override;
  INodeRender* NextChild(const size_t&) const override;
  size_t TotalChild() const override;
  void Push(INodeRender*) override;
  std::uintptr_t NodeKey() const override;
  void NodeKey(const std::uintptr_t&) override;
  const IResources* GetRes() const override;
  PSIZE Size() const override;
  PRECT SizeBox() const override;
  PRECT Caption() const override;
  PSIZE RoundCorner() const override;
  PSIZE Mininfo() const override;
  PSIZE Maxinfo() const override;
  PSIZE BorderRound() const override;
  PPOINT Pos() const override;
  TypeArgbType BkColor() const override;
  TypeArgbType BorderColor() const override;
  ARGB BkColorArgb() const override;
  ARGB BorderColorArgb() const override;
  int BorderSize() const override;
  int Width() const override;
  int Height() const override;
  const IStrings* Name() const override;
  const IStrings* BkImage() const override;
  void BkImage(const tfCacheCb&) const override;
  const IStrings* Text() const override;
  bool Visible() const override;
  const IStrings* LogoPathname() const override;
  void Logo(const tfCacheCb&) const override;
  HICON LogoIcon() const override;
  float Rounding() const override;
  bool Floating() const override;
  float Alpha() const override;
  const IStrings* FontFilePathname() const override;
  float Pixels() const override;
  bool Bold() const override;
  void FontCache(const tfCacheCb&) const override;
  TypeIdentifyType FontIdentify() const override;
  void Route(void*) override;
  void* Route() const override;
  void Handle(void*) override;
  void* Handle() const override;
  bool FlagInitialization() const override;
  void FlagInitialization(const bool&) override;
  const IStrings* ClassName() const override;
  void FontHandle(void*) override;
  void* FontHandle() const override;
  LayoutType Layout() const override;
 public:
  void DefaultConfigCover() override;
 public:
  const tfSkinNodes& Childs() const;
 protected:
  TypeIdentifyType identify_ = 0;
  tfSkinNodes child_node_s_;
  const ControlType control_type_;
  INodeRender* parent_ = nullptr;
  std::uintptr_t node_key_ = 0;
  const IResources* resources_;
  tagPOINT pos_ = { 0 };
  tagRECT sizebox_ = { 0 };
  tagRECT caption_ = { 0 };
  tagSIZE size_ = { 0 };
  tagSIZE borderround_ = { 0 };
  tagSIZE roundcorner_ = { 0 };
  tagSIZE mininfo_ = { 0 };
  tagSIZE maxinfo_ = { 0 };
  TypeArgbType bkcolor_ = 0;
  TypeArgbType bordercolor_ = 0;
  int bordersize_ = 0;
  int width_ = 0;
  int height_ = 0;
  bool visible_ = true;
  HICON logo_icon_ = nullptr;
  float rounding_ = 0.0f;
  float alpha_ = 1.0f;
  bool floating_ = false;
  float pixels_ = 10.0f;
  bool bold_ = false;
  TypeIdentifyType font_ = 0;
  void* route_ = nullptr;
  void* handle_ = nullptr;
  void* font_handle_ = nullptr;
  bool flag_initialization_ = false;
  TypeCacheType font_cache_;
  TypeCacheType bkimage_cache_;
  TypeCacheType logo_cache_;
  Strings class_name_;
  Strings fontfile_pathname_;
  Strings bkimage_pathname_;
  Strings logo_pathname_;
  Strings path_;
  Strings name_;
  Strings text_;
  LayoutType layout_ = LayoutType::Floating;
 };


 class Window : public NodeRender {
 public:
  Window(const TypeIdentifyType&, const IResources*);
  virtual ~Window();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class Frame : public NodeRender {
 public:
  Frame(const TypeIdentifyType&, const IResources*);
  virtual ~Frame();
  void Release() const override final;
 };

 class Control : public NodeRender {
 public:
  Control(const TypeIdentifyType&, const IResources*);
  virtual ~Control();
  void Release() const override final;
 };

 class Container : public NodeRender {
 public:
  Container(const TypeIdentifyType&, const IResources*);
  virtual ~Container();
  void Release() const override final;
 };

 class VerticalLayout : public NodeRender {
 public:
  VerticalLayout(const TypeIdentifyType&, const IResources*);
  virtual ~VerticalLayout();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class HorizontalLayout : public NodeRender {
 public:
  HorizontalLayout(const TypeIdentifyType&, const IResources*);
  virtual ~HorizontalLayout();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class TabLayout : public NodeRender {
 public:
  TabLayout(const TypeIdentifyType&, const IResources*);
  virtual ~TabLayout();
  void Release() const override final;
 };

 class Button : public NodeRender {
 public:
  Button(const TypeIdentifyType&, const IResources*);
  virtual ~Button();
  void Release() const override final;
 };

 class Label : public NodeRender {
 public:
  Label(const TypeIdentifyType&, const IResources*);
  virtual ~Label();
  void Release() const override final;
 };

 class Font : public NodeRender {
 public:
  Font(const TypeIdentifyType&, const IResources*);
  virtual ~Font();
  void Release() const override final;
 };

}///namespace local




/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 23:41:25 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__828DAEA0_960A_4F3A_A9F0_0F14FF281869__