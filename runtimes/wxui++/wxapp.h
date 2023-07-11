#if !defined(__4E69A9CF_CC2F_4DEF_A12B_488027B7D061__)
#define __4E69A9CF_CC2F_4DEF_A12B_488027B7D061__

namespace local {

 class IApp
  : public wxApp {
  friend class Wxui;
 public:
  IApp(Wxui*);
  virtual ~IApp();
 protected:
  int OnExit() override final;
  bool OnInit() override final;
 public:
  Wxui* WxuiGet() const;
 protected:
  Wxui* m_pWxui = nullptr;
 private:
  void OnAppDestory(wxThreadEvent& event);
 };

 extern const int WX_MSG_ON_APP_DESTROY;
}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 02:26:46 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__4E69A9CF_CC2F_4DEF_A12B_488027B7D061__