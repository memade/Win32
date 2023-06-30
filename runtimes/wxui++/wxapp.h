#if !defined(__4E69A9CF_CC2F_4DEF_A12B_488027B7D061__)
#define __4E69A9CF_CC2F_4DEF_A12B_488027B7D061__

namespace local {

 class App
  : public wxApp
  , public IApp {
  friend class Wxui;
 public:
  App(Wxui*);
  virtual ~App();
 protected:
  int OnExit() override final;
  bool OnInit() override final;
 public:
  Wxui* WxuiGet() const;
 protected:
  void RegisterOnAppInitCb(const tfOnAppInitCb&) override final;
  void RegisterOnAppUninitCb(const tfOnAppUninitCb&) override final;
  void RegisterOnAppCreateFrameCb(const tfOnAppCreateFrameCb&) override final;
 protected:
  Wxui* m_pWxui = nullptr;
  tfOnAppInitCb m_OnAppInitCb = nullptr;
  tfOnAppUninitCb m_OnAppUninitCb = nullptr;
  tfOnAppCreateFrameCb m_OnAppCreateFrameCb = nullptr;
 private:
  void OnAppInit(const bool& result) const;
  void OnAppUninit(const int& exit_code) const;
  void OnAppCreateFrame(IFrame* frame) const;
  void OnAppDestory(wxThreadEvent& event);
 };

 extern const int WX_MSG_ON_APP_DESTROY;
}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 02:26:46 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__4E69A9CF_CC2F_4DEF_A12B_488027B7D061__