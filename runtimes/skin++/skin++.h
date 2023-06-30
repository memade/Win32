#if !defined(__C40A8379_718F_4F38_85BC_078872DAECC4__)
#define __C40A8379_718F_4F38_85BC_078872DAECC4__

namespace local {

 class Skin final : public ISkin {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Skin();
  virtual ~Skin();
 private:
  void Init();
  void UnInit();
 public:
  void Release() const override final;
  bool Perform() override final;
  void Render() override final;
  void From(ISkinUI*) override final;
  bool From(const char*) override final;
  INodeRender* MainNode() const override final;
  void RegisterNodeCreateCb(const tfNodeCreateCb&) override final;
  void RegisterNodeDestroyCb(const tfNodeDestroyCb&) override final;
  void NodeIterator(const tfNodeBeginCb&, const tfNodeDestroyCb&) const override final;
  void FontNodeIterator(const tfNodeCb&) const override final;
  void ParserArgb(const TypeArgbType&, std::uint8_t&, std::uint8_t&, std::uint8_t&, std::uint8_t&) const override final;
 private:
  void OnNodeCreate(INodeRender*) const;
  void OnNodeDestroy(INodeRender*) const;
 private:
  std::atomic_bool m_IsOpen = false;
  ISkinUI* m_pUI = nullptr;
  std::string m_SkinDirectory;
  tfSkinNodes m_Nodes;
  tfResNodes m_ResNodes;
  INodeRender* m_pMainNode = nullptr;
  tfNodeDestroyCb m_NodeDestroyCb = nullptr;
  tfNodeCreateCb m_NodeCreateCb = nullptr;
  tfFontNodes m_FontNodes;
  std::vector<std::thread> m_Threads;
 };



 extern HINSTANCE __gpHinstance;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 00:26:33 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C40A8379_718F_4F38_85BC_078872DAECC4__
