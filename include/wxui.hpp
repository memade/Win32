/*===- wxui.hpp - Wrapper for 'extern "C"' ---------------*- C -*-=======*\
|*                                                                                                                       *|
|* Part of the wxui® Project, under the Apache License v2.0 with wxui®  *|
|* Exceptions.                                                                                                     *|
|* See http://skstu.com/wxui/LICENSE.txt for license information.                   *|
|* SPDX-License-Identifier: Apache-2.0 WITH wxui-exception                        *|
|*                                                                                                                        *|
|*===----------------------------------------------------------------------====== *|
|*                                                                                                                        *|
|* This file is a summary of the interface for this project.                                      *|
|*                                                                                                                        *|
\*===----------------------------------------------------------------------======*/
#if !defined(__E6A92BDF_7BBD_4C14_B94F_47BE0C1A43F6__)
#define __E6A92BDF_7BBD_4C14_B94F_47BE0C1A43F6__

#include <dllinterface.hpp>

namespace wxui {
 class IFrame;
 class IChildFrame;
 class IApp;

 enum class FrameType : unsigned long {
  SDI= 0x0000,
  WEBVIEW = 0x0010,
  MDI = 0x0100,
 };

 enum class FrameStyleType : long {
  //wxDEFAULT_FRAME_STYLE | wxFRAME_NO_WINDOW_MENU /*wxVSCROLL | wxHSCROLL*/,
 };

 enum class ChildStyleType : long {

 };

 class IConfig : public shared::IUIConfig {
 public:
  virtual FrameType FrameTypeGet() const = 0;
  virtual void FrameTypeSet(const FrameType&) = 0;
 };

 class IChildFrame {
 public:
  virtual void Show(const bool&) = 0;
  virtual HWND Hwnd() const = 0;
  virtual void Maximize() = 0;
 };

 class IFrame {
 protected:
  using tfOnFrameCreateChildCb = std::function<void(IChildFrame*)>;
  using tfOnFrameCreateChildHostCb = std::function<void(IChildFrame*)>;
 public:
  virtual void Show(const bool&) = 0;
  virtual HWND Hwnd() const = 0;
  virtual IChildFrame* CreateChildFrame() = 0;
  virtual IChildFrame* CreateChildFrameHost(const bool&) = 0;
  virtual void RegisterOnFrameCreateChildCb(const tfOnFrameCreateChildCb&) = 0;
  virtual void RegisterOnFrameCreateChildHostCb(const tfOnFrameCreateChildHostCb&) = 0;
  virtual void MDIWindowCascade() {}
  virtual void MDIWindowTileHorz() {}
  virtual void MDIWindowTileVert() {}
  virtual bool MDIAppendChild(const HWND&) { return false; }
 };

 class IApp {
 protected:
  using tfOnAppInitCb = std::function<void(const bool&)>;
  using tfOnAppUninitCb = std::function<void(const int& /*wx exit code.*/)>;
  using tfOnAppCreateFrameCb = std::function<void(IFrame*)>;
 public:
  virtual void RegisterOnAppInitCb(const tfOnAppInitCb&) = 0;
  virtual void RegisterOnAppUninitCb(const tfOnAppUninitCb&) = 0;
  virtual void RegisterOnAppCreateFrameCb(const tfOnAppCreateFrameCb&) = 0;
 };

 class IWxui : public shared::InterfaceDll<IWxui> , public shared::IUserInterface {
 public:
  using tfOnAppCreateCb = std::function<void(IApp*)>;
 public:
  virtual IConfig* ConfigGet() const { return nullptr; }
  virtual IFrame* FrameGet() const { return nullptr; }
  virtual void RegisterOnAppCreateCb(const tfOnAppCreateCb&) { return; }
 };

}///namespace wxui

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 26 May 2023 16:37:56 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__E6A92BDF_7BBD_4C14_B94F_47BE0C1A43F6__



