#if !defined(__3620E2E3_EB92_48C9_8FC3_90C33A0E0CC6__)
#define __3620E2E3_EB92_48C9_8FC3_90C33A0E0CC6__

namespace local {

 class MDIFrame final : public wxMDIParentFrame, public IFrame {
 public:
  MDIFrame(
   const IWxui* host,
   wxWindow* parent = nullptr,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(MDIFrame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxDEFAULT_FRAME_STYLE /*| wxFRAME_NO_WINDOW_MENU*/ /*wxVSCROLL | wxHSCROLL*/,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~MDIFrame();
 protected:
  void Show(const bool&) override final;
  void Size(const ISize*) override final;
  void Pos(const IPos*) override final;
  void Align(const IAlign*) override final;
  void* Handle() const override final;
 private:
  void OnCloseWindow(wxCloseEvent& wxEvent);
  void OnSize(wxSizeEvent& wxEvent);
  bool show_ = false;
  const IWxui* host_;
 };


}///namespace local



/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 24 Jun 2023 01:28:00 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__3620E2E3_EB92_48C9_8FC3_90C33A0E0CC6__