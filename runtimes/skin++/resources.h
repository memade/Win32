#if !defined(__DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__)
#define __DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__

namespace local {

 class Resources : public IResources {
 public:
  Resources(const ResourcesType&);
  virtual ~Resources();
 public:
  void Release() const override;
  const ResourcesType& Type() const override;
  const char* Path() const override;
  const char* Pathname() const override;
  void Cache(const tfCacheCb&) const override;
  void Path(const char*) override;
  void Pathname(const char*) override;
  void Cache(const char*, const size_t&) override;
  bool Load() override;
 public:
  const std::string& Cache() const;
 protected:
  std::string path_;
  std::string pathname_;
  std::string cache_;
  const ResourcesType type_;
 };

 class ResourcesConfigXml final : public Resources {
 public:
  ResourcesConfigXml();
  virtual ~ResourcesConfigXml();
 };

 class ResourcesConfigJson final : public Resources {
 public:
  ResourcesConfigJson();
  virtual ~ResourcesConfigJson();
 };

 class ResourcesImageIcon final : public Resources {
 public:
  ResourcesImageIcon();
  virtual ~ResourcesImageIcon();
 };

 class ResourcesFontTTF final : public Resources {
 public:
  ResourcesFontTTF();
  virtual ~ResourcesFontTTF();
 };

 class ResourcesFontTTC final : public Resources {
 public:
  ResourcesFontTTC();
  virtual ~ResourcesFontTTC();
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 26 Jun 2023 22:14:45 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__