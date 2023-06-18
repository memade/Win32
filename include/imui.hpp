#if !defined(__9DDFE096_6FC1_4FCD_A9E4_7235F9965F5A__)
#define __9DDFE096_6FC1_4FCD_A9E4_7235F9965F5A__

#include <dllinterface.hpp>

namespace imui {

 class IConfig : public shared::IUIConfig {
 public:

 };

 class IImui : public shared::InterfaceDll<IImui>, public shared::IUserInterface {
 protected:
  using tfOnCreateWindowCb = std::function<void(\
    DWORD& dwExStyle,
    LPCWSTR& lpClassName,
    LPCWSTR& lpWindowName,
    DWORD& dwStyle,
    int& X,
    int& Y,
    int& nWidth,
    int& nHeight,
    HWND& hWndParent,
    HMENU& hMenu,
    HINSTANCE& hInstance,
    LPVOID& lpParam)>;
  using tfOnCreateWindowClassCb = std::function<void(PWNDCLASSEXW)>;
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual void RegisterOnCreateWindowCb(const tfOnCreateWindowCb&) = 0;
  virtual void RegisterOnCreateWindowClassCb(const tfOnCreateWindowClassCb&) = 0;
 };




}///namespace imui

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 07:37:42 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__9DDFE096_6FC1_4FCD_A9E4_7235F9965F5A__