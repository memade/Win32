/*===- libuvpp.hpp - Wrapper for 'extern "C"' ---------------*- C -*-=======*\
|*                                                                                                                       *|
|* Part of the Libuvpp® Project, under the Apache License v2.0 with Libuvpp®  *|
|* Exceptions.                                                                                                     *|
|* See http://skstu.com/libuvpp/LICENSE.txt for license information.                   *|
|* SPDX-License-Identifier: Apache-2.0 WITH LIBUVPP-exception                        *|
|*                                                                                                                        *|
|*===----------------------------------------------------------------------====== *|
|*                                                                                                                        *|
|* This file is a summary of the interface for this project.                                      *|
|*                                                                                                                        *|
\*===----------------------------------------------------------------------======*/
#if !defined(__36C78677_A6CF_4DDF_8BAF_B7A90BB6D196__)
#define __36C78677_A6CF_4DDF_8BAF_B7A90BB6D196__

#include <dllinterface.hpp>

namespace ssl {

 class IConfig {
 public:

 };

 class ISsl : public shared::InterfaceDll<ISsl> {
 protected:
  using tfResultDataCb = std::function<void(const char*, size_t)>;
 public:
  virtual void Release() const = 0;
  virtual IConfig* ConfigGet() const = 0;
  virtual bool EnMD5(const char*, size_t, const tfResultDataCb&) const = 0;
  virtual bool EnBase64(const char*, size_t, const bool& newline, const tfResultDataCb&) const = 0;
  virtual bool DeBase64(const char*, size_t, const bool& newline, const tfResultDataCb&) const = 0;
 };

}///namespace ssl

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 10 Jun 2023 11:09:05 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__36C78677_A6CF_4DDF_8BAF_B7A90BB6D196__




