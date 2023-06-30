#if !defined(__648139CC_5E52_431F_8A1E_CB96EFB5852D__)
#define __648139CC_5E52_431F_8A1E_CB96EFB5852D__

namespace local {
#if 0
 class ChildFrame final : public IChildFrame, public wxMDIChildFrame {
 public:
  ChildFrame(wxMDIParentFrame* parent,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(ChildFrame®)",
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxDEFAULT_FRAME_STYLE,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~ChildFrame();
 protected:
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  void Maximize() override final;
 private:
  void OnSize(wxSizeEvent&);
  void OnSizing(wxSizeEvent&);
  void OnCloseWindow(wxCloseEvent&);
 };

 class ChildFrameHost final : public IChildFrame, public wxMDIChildFrame {
 public:
  ChildFrameHost(wxMDIParentFrame* parent,
   wxWindowID id = wxID_ANY,
   const wxString& title = LR"(ChildFrame®)",
   const wxPoint& pos = /*wxDefaultPosition*/wxPoint(0, 0),
   const wxSize& size = /*wxDefaultSize*/wxSize(200, 200),
   long style = /*wxDEFAULT_FRAME_STYLE*//*wxNO_BORDER | wxFRAME_SHAPED*/
wxSYSTEM_MENU | 
             /*wxRESIZE_BORDER | */
             /*wxMINIMIZE_BOX | 
             wxMAXIMIZE_BOX | */
             /*wxCLOSE_BOX | 
             wxCAPTION | */
             wxCLIP_CHILDREN,
   const wxString& name = wxASCII_STR(wxFrameNameStr));
  virtual ~ChildFrameHost();
 protected:
  void Show(const bool&) override final;
  HWND Hwnd() const override final;
  void Maximize() override final;
 private:
  void ReSetShape();
  bool SetShape();
  void OnSize(wxSizeEvent&);
  void OnSizing(wxSizeEvent&);
  void OnCloseWindow(wxCloseEvent&);
  void OnPaint(wxPaintEvent&);
  void OnErase(wxEraseEvent&);
 };
#endif
}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 02:27:08 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__648139CC_5E52_431F_8A1E_CB96EFB5852D__