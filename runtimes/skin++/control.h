#if !defined(__828DAEA0_960A_4F3A_A9F0_0F14FF281869__)
#define __828DAEA0_960A_4F3A_A9F0_0F14FF281869__

namespace local {

 class Control : public IControlSK {
 public:
  Control(const ControlType&, const String& skin_root_path = "");
  virtual ~Control();
 public:
  virtual const IControls* Childs() const;
  virtual const std::uintptr_t& NodeKey() const;
  virtual const std::uintptr_t& ParentNodeKey() const;
  virtual void NodeKey(const std::uintptr_t&);
  virtual void ParentNodeKey(const std::uintptr_t&);
  virtual void operator<<(const std::map<AttributeType, String>&);
  virtual void SetAttribute(const AttributeType&, const tfAttributeValueType&);
 public:
  void PushChild(IControl*) override;
  const IAttributes* GetAttributes() const override;
  Vec2 GetSize() const override;
  Vec2 GetPosition() const override;
  bool IsFloating() const override;
 protected:
  String skin_root_path_;
  std::uintptr_t node_key_ = 0;
  std::uintptr_t parent_node_key_ = 0;
  IControls* child_s_ = nullptr;
 };

 class Controls final : public IControls {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Controls();
  virtual ~Controls();
 protected:
  void Release() const override final;
  bool Empty() const override final;
  unsigned long Total() const override final;
  void Push(IControl*) override final;
  IControl* Next(const unsigned long&) const override final;
  void Iterator(const std::function<void(IControl*, bool&)>&) const override final;
  void LayoutIterator(const std::function<void(IControl*, bool&)>&) const override final;
  unsigned long LayoutCount() const override final;
 private:
  std::vector<IControl*> controls_;
 };

 class Window final : public Control {
 public:
  Window();
  virtual ~Window();
  void Release() const override final;
 };

 class Frame final : public Control {
 public:
  Frame();
  virtual ~Frame();
  void Release() const override final;
 };

 class VerticalLayout final : public Control {
 public:
  VerticalLayout();
  virtual ~VerticalLayout();
  void Release() const override final;
 };

 class HorizontalLayout final : public Control {
 public:
  HorizontalLayout();
  virtual ~HorizontalLayout();
  void Release() const override final;
 };

 class Button final : public Control {
 public:
  Button();
  virtual ~Button();
  void Release() const override final;
 };

 class Font final : public Control {
 public:
  Font(const String&);
  virtual ~Font();
  void Release() const override final;
 };

 class Image final : public Control {
 public:
  Image(const String&);
  virtual ~Image();
  void Release() const override final;
 };

 class UserData final : public Control {
 public:
  UserData(const String&);
  virtual ~UserData();
  void Release() const override final;
 };








#if 0

 class Node : public INodeSK {
 protected:
  Node(const TypeIdentifyType&, const IResources*, const ControlType&);
  virtual ~Node();
 public:
  virtual bool operator<<(const std::array<String, 2>&);
 protected:
  void NodeUI(INode*) override;
  void NodeUI(INodeUI*) override;
  INodeUI* NodeUI() const override;
  void Parent(INodeSK*) override;
  INodeSK* Parent() const override;
 protected:
  const TypeIdentifyType& Identify() const override;
  const ControlType& GetControlType() const override;
  INode* BeginChild() const override;
  INode* EndChild() const override;
  INode* NextChild(const size_t&) const override;
  size_t TotalChild() const override;
  void Push(INodeSK*) override;
  std::uintptr_t NodeKey() const override;
  void NodeKey(const std::uintptr_t&) override;
  const IResources* GetResources() const override;
  const Vec2& Size() const override;
  const Vec4& SizeBox() const override;
  const Vec4& Caption() const override;
  const Vec2& RoundCorner() const override;
  const Vec2& MinSize() const override;
  const Vec2& MaxSize() const override;
  const Vec2& BorderRound() const override;
  const Vec2& Position() const override;
  const TypeArgbType& BkColor() const override;
  const TypeArgbType& BorderColor() const override;
  Vec4 BkColorArgb() const override;
  Vec4 BorderColorArgb() const override;
  const TypePixelsType& BorderSize() const override;
  const IString* Name() const override;
  const IString* BkImage() const override;
  void BkImage(const tfCacheCb&) const override;
  const IString* Text() const override;
  const bool& Visible() const override;
  const IString* LogoPathname() const override;
  void Logo(const tfCacheCb&) const override;
  HICON LogoIcon() const override;
  const TypePixelsType& Rounding() const override;
  const bool& Floating() const override;
  const TypePixelsType& Alpha() const override;
  const TypeIdentifyType& Font() const override;
  const IString* ClassName() const override;
  const LayoutType& Layout() const override;
  const bool& Maximize() const override;
 public:
  void DefaultConfigCover() override;
 public:
  const std::vector<INodeSK*>& Childs() const;
  void ClearAttributeMap();
  const std::map<AttributeType, String>& AttributeMap() const;
 protected:
  TypeIdentifyType identify_ = 0;
  std::vector<INodeSK*> child_node_s_;
  const ControlType control_type_;
  std::uintptr_t node_key_ = 0;
  const IResources* resources_;
  Vec2 position_;
  Vec4 sizebox_;
  Vec4 caption_;
  Vec2 size_;
  Vec2 borderround_;
  Vec2 roundcorner_;
  Vec2 minsize_;
  Vec2 maxsize_;
  TypeArgbType bkcolor_ = 0;
  TypeArgbType bordercolor_ = 0;
  TypePixelsType bordersize_ = 0;
  bool visible_ = true;
  HICON logo_icon_ = nullptr;
  TypePixelsType rounding_ = 0.0f;
  TypePixelsType alpha_ = 1.0f;
  bool floating_ = false;
  bool bold_ = false;
  TypeIdentifyType font_ = 0;
  TypeCacheType bkimage_cache_;
  TypeCacheType logo_cache_;
  String class_name_;
  String bkimage_pathname_;
  String logo_pathname_;
  String path_;
  String name_;
  String text_;
  bool maximize_ = false;
  std::map<AttributeType, String> attribute_map_;
  LayoutType layout_ = LayoutType::Floating;
 };


 class Window : public Node {
 public:
  Window(const TypeIdentifyType&, const IResources*);
  virtual ~Window();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class Frame : public Node {
 public:
  Frame(const TypeIdentifyType&, const IResources*);
  virtual ~Frame();
  void Release() const override final;
 };

 class Control : public Node {
 public:
  Control(const TypeIdentifyType&, const IResources*);
  virtual ~Control();
  void Release() const override final;
 };

 class Container : public Node {
 public:
  Container(const TypeIdentifyType&, const IResources*);
  virtual ~Container();
  void Release() const override final;
 };

 class VerticalLayout : public Node {
 public:
  VerticalLayout(const TypeIdentifyType&, const IResources*);
  virtual ~VerticalLayout();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class HorizontalLayout : public Node {
 public:
  HorizontalLayout(const TypeIdentifyType&, const IResources*);
  virtual ~HorizontalLayout();
  void Release() const override final;
  void DefaultConfigCover() override final;
 };

 class TabLayout : public Node {
 public:
  TabLayout(const TypeIdentifyType&, const IResources*);
  virtual ~TabLayout();
  void Release() const override final;
 };

 class Button : public Node {
 public:
  Button(const TypeIdentifyType&, const IResources*);
  virtual ~Button();
  void Release() const override final;
 };

 class Label : public Node {
 public:
  Label(const TypeIdentifyType&, const IResources*);
  virtual ~Label();
  void Release() const override final;
 };

 class Font : public Node, public skin::IFont {
 public:
  Font(const TypeIdentifyType&, const IResources*);
  virtual ~Font();
  void Release() const override final;
 protected:
  const TypeIdentifyType& Identify() const override final;
  const bool& Bold() const override final;
  const ICache* GetCache() const override final;
  void* Handle() const override final;
  const IString* FilePathname() const override final;
 private:
  TypeIdentifyType identify_ = 0;
  Cache font_cache_;
  String file_pathname_;
  bool bold_ = false;
 };
#endif
}///namespace local




/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 23:41:25 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__828DAEA0_960A_4F3A_A9F0_0F14FF281869__