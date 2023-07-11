#if !defined(__7EAC3FFF_07B8_4C84_93A0_3438218AFD17__)
#define __7EAC3FFF_07B8_4C84_93A0_3438218AFD17__

namespace local {

 using tfAttributeValueType = std::variant<bool, Vec2, Vec4, TypePixels, TypeIdentify, IString*, ICache*>;

 class Attribute final : public IAttribute {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Attribute(const Attribute&);
  Attribute(const ControlType&, const AttributeType&);
  Attribute(const ControlType&, const AttributeType&, const tfAttributeValueType&);
  virtual ~Attribute();
 public:
  void operator=(const Attribute&);
  void operator<<(const tfAttributeValueType&);
  bool SetValue(const String&);
  void SetSkinRootPath(const String&);
 public:
  void Release() const override final;
  bool GetBool() const override final;
  Vec2 GetVec2() const override final;
  Vec4 GetVec4() const override final;
  TypeIdentify GetIdentify() const override final;
  TypePixels GetFloat() const override final;
  const ICache* GetCache() const override final;
  const IString* GetString() const override final;
  const AttributeType& GetType() const override final;
   const IControl* ResourceRef() const override final;
   void ResourceRef(const IControl*) override final;
 private:
  template<typename T>
  void operator>>(T& output) const {
   if (std::holds_alternative<T>(value_))
    output = std::get<T>(value_);
  }
 private:
  String skin_root_path_;
  const ControlType control_type_;
  const AttributeType attribute_type_;
  tfAttributeValueType value_;
  IControl* resource_ref_ = nullptr;
 };

 class Attributes final : public IAttributes {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Attributes(const String&, const ControlType&);
  virtual ~Attributes();
  void operator<<(const std::map<AttributeType, String>&);
 public:
  void Release() const override final;
  const ControlType& GetControlType() const override final;
  const IAttribute* GetAttribute(const AttributeType&) const override final;
  void Push(const IAttribute*) override final;
  void Push(const Attribute&);
  unsigned long Total() const override final;
  const IAttribute* Next(const unsigned long&) const override final;
  bool Empty() const override final;
  Vec2 GetSize(const AttributeType&) const override final;
  const ICache* GetCache(const AttributeType&) const override final;
  const IString* GetTitleName(const AttributeType&) const override final;
  Vec4 GetBkColor(const AttributeType&) const override final;
  void Iterator(const std::function<void(const AttributeType&, IAttribute*, bool&)>&) const override final;
 private:
  const String skin_root_path_;
  const ControlType control_type_;
  void operator<<(const IAttribute*);
  void operator<<(const Attribute&);
  std::vector<IAttribute*> attributes_;
 };
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 03 Jul 2023 15:28:38 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__7EAC3FFF_07B8_4C84_93A0_3438218AFD17__