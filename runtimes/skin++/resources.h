#if !defined(__DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__)
#define __DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__

namespace local {

 class Resources final : public IResources {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Resources();
  virtual ~Resources();
 public:
  void Release() const override final;
  void Push(IResource*) override final;
  void Pop(const TypeIdentify&) override final;
  void Clear() override final;
  IResource* Search(const TypeIdentify&) const override final;
 private:
  std::map<TypeIdentify, IResource*> resources_;
 };

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 26 Jun 2023 22:14:45 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DC01BDAE_6F01_4829_A5DC_EC6AD701D7F7__