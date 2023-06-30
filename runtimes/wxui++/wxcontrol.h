#if !defined(__1A0DB085_4C94_4235_B700_62A77727CEAA__)
#define __1A0DB085_4C94_4235_B700_62A77727CEAA__

namespace local {

 class TreeList final : public wxTreeListCtrl, public ITreeList {
 public:
  TreeList(wxWindow* parent,
   wxWindowID id = wxID_ANY,
   const wxPoint& pos = wxDefaultPosition,
   const wxSize& size = wxDefaultSize,
   long style = wxTL_DEFAULT_STYLE,
   const wxString& name = wxASCII_STR(wxTreeListCtrlNameStr));
  virtual ~TreeList();
 protected:
  void Show(const bool&) override final;
  void Size(const ISize*) override final;
  void Pos(const IPos*) override final;
  void Align(const IAlign*) override final;
  void* Handle() const override final;
 private:
  void Init();
  void UnInit();
 };



}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 24 Jun 2023 01:17:44 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__1A0DB085_4C94_4235_B700_62A77727CEAA__