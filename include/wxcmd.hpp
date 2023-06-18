/*===- wxcmd.hpp - Wrapper for 'extern "C"' ---------------*- C -*-=======*\
|*                                                                                                                       *|
|* Part of the wxcmd® Project, under the Apache License v2.0 with wxcmd®  *|
|* Exceptions.                                                                                                     *|
|* See http://skstu.com/wxcmd/LICENSE.txt for license information.                   *|
|* SPDX-License-Identifier: Apache-2.0 WITH wxcmd-exception                        *|
|*                                                                                                                        *|
|*===----------------------------------------------------------------------====== *|
|*                                                                                                                        *|
|* This file is a summary of the interface for this project.                                      *|
|*                                                                                                                        *|
\*===----------------------------------------------------------------------======*/
#if !defined(__BB387F4A_8C3E_45CB_AC42_020B3F38C129__)
#define __BB387F4A_8C3E_45CB_AC42_020B3F38C129__

#include <dllinterface.hpp>

namespace cmd {

 class IParser {
 public:
  using tfOnAllcoStringCb = std::function<void(const char*)>;
 public:
  virtual void SetCmdLine(const char*) = 0;
  virtual void Release() const = 0;
  virtual bool Ready() const = 0;
  virtual bool Perform() = 0;
  virtual void SetSwitchChars(const char*) = 0;
  virtual void SetLogo(const char*) = 0;
  virtual void AddSwitch(const char*) = 0;
  virtual void AddLongSwitch(const char*) = 0;
  virtual void AddOption(const char* option, const char* value) = 0;
  virtual void AddLongOption(const char* name, const char* value) = 0;
  virtual void AddParam(const char*) = 0;
  virtual void GetParam(const size_t&, const tfOnAllcoStringCb&) const = 0;
  virtual bool Found(const char* name) const = 0;
  virtual bool FoundSwitch(const char* name) const = 0;
  virtual bool Found(const char* name, const tfOnAllcoStringCb&) const = 0;
  virtual bool Found(const char* name, long& value) const = 0;
  virtual bool Found(const char* name, double& value) const = 0;
 };
 class ICmd : public shared::InterfaceDll<ICmd> {
 public:
  virtual void Release() const = 0;
  virtual IParser* ParserCreate() = 0;
  virtual bool ParserDestory(IParser*&) = 0;
 };

}///namespace cmd

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 30 May 2023 06:26:19 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__BB387F4A_8C3E_45CB_AC42_020B3F38C129__




