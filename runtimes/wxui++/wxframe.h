#if !defined(__D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__)
#define __D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__

namespace local {

 class IFrame final : public wxFrame {
 public:
  IFrame(const IWxui* host,
   wxWindow* parent = nullptr,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(Frame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxDEFAULT_FRAME_STYLE,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~IFrame();
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
  bool show_ = false;
  const IWxui* host_;
 };

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 02:26:56 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__D7C8EBE9_D2FE_4D67_A3D8_0D36121717B6__