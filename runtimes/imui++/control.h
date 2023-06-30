#if !defined(__CC3E7B38_9189_4884_BABE_D1267B8B408C__)
#define __CC3E7B38_9189_4884_BABE_D1267B8B408C__

namespace local {

 class ICtrl {
 public:
  ICtrl(skin::INodeRender*);
  ~ICtrl();
 public:
  virtual void Release() const = 0;
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual skin::INodeRender* Node() const;
  virtual void OnLayout() const;
 protected:
  skin::INodeRender* node_ = nullptr;
  bool flag_open_ = true;
 };

 class Window final : public ICtrl {
 public:
  Window(skin::INodeRender*);
  virtual ~Window();
  void Release() const override final;
  void Begin() override final;
  void End() override final;
 };

 class Font final : public ICtrl {
 public:
  Font(skin::INodeRender*);
  virtual ~Font();
  void Release() const override final;
  void Begin() override final;
  void End() override final;
 };

 class Frame final : public ICtrl {
 public:
  Frame(skin::INodeRender*);
  virtual ~Frame();
  void Release() const override final;
  void Begin() override final;
  void End() override final;
 };

 class VerticalLayout final : public ICtrl {
 public:
  VerticalLayout(skin::INodeRender*);
  virtual ~VerticalLayout();
  void Release() const override final;
  void Begin() override final;
  void End() override final;
 };

 class HorizontalLayout final : public ICtrl {
 public:
  HorizontalLayout(skin::INodeRender*);
  virtual ~HorizontalLayout();
  void Release() const override final;
  void Begin() override final;
  void End() override final;
 };

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 28 Jun 2023 00:01:37 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__CC3E7B38_9189_4884_BABE_D1267B8B408C__