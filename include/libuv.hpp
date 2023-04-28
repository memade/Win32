/*===- libuv.hpp - Wrapper for 'extern "C"' ---------------*- C -*-=======*\
|*                                                                                                                       *|
|* Part of the Libuv® Project, under the Apache License v2.0 with Libuv®  *|
|* Exceptions.                                                                                                     *|
|* See http://skstu.com/libuv/LICENSE.txt for license information.                   *|
|* SPDX-License-Identifier: Apache-2.0 WITH LIBUV-exception                        *|
|*                                                                                                                        *|
|*===----------------------------------------------------------------------====== *|
|*                                                                                                                        *|
|* This file is a summary of the interface for this project.                                      *|
|*                                                                                                                        *|
\*===----------------------------------------------------------------------======*/
#if !defined(__46FA06EA_358C_4885_AF2C_24AFCB15616F__)
#define __46FA06EA_358C_4885_AF2C_24AFCB15616F__


#include <dllinterface.hpp>

namespace libuv {

 using TypeIdentify = unsigned long long;
 using tf_api_object_init = void* (__stdcall*)(const void*, unsigned long);
 using tf_api_object_uninit = void(__stdcall*)(void);

 enum class UVIPVType : unsigned long {
  IPV4 = 0,
  IPV6
 };

 enum class EnServerType : unsigned long {
  TCP = 0,
  UDP,
  PIPE
 };

 enum class EnSessionType : unsigned long {
  TCP_SESSION_CLIENT = 0x110,
  TCP_SESSION_SERVER = 0x120,
  UDP_SESSION_CLIENT = 0x210,
  UDP_SESSION_SERVER = 0x220,
  PIPE_SESSION_CLIENT = 0x310,
  PIPE_SESSION_SERVER = 0x320,
 };

 // Enum for different connection status types with integer values
 typedef enum class ConnectionStatus : unsigned long {
  Closed = 0, //!@ Connection is closed and not opened yet
  Opening = 1, //!@ Connection is being opened
  Opened = 2, //!@ Connection is opened successfully
  Connecting = 3, //!@ Connection is being established
  Disconnected = 4, //!@ Connection is completed with failure
  Connected = 5, //!@ Connection is completed successfully
  Ready = 6, //!@ Connection is ready
  ForceClose = 7, //!@ Close command has been initiated
  Closing = 8, //!@ Connection is being closed
  Unready = 9,//!@ opening failed.
  ConnectReset = 0xA,//!@ reset action.
  ConnectExit = 0xB,//!@ module exit.

  Begin = Closed,
  End = ConnectExit,
 }ClientStatus, ServerStatus, SessionStatus;

 enum class StreamType : unsigned long {
  Read = 0,
  Write,
 };

 enum class ZipType : unsigned long {
  UNDEFINE = 0,
  ZIP,
  GZIP,
 };

 enum class EncryptType : unsigned long {
  UNDEFINE = 0,
  WEMADE,
 };

 enum class CommandType : unsigned long {
  Welcome = 0x10000,
  Hello = Welcome,
  HeartBeat = 0x10100,
  KeepAlive = HeartBeat,
 };

 struct IPacketHeader {
  virtual ZipType Zip() const = 0;
  virtual EncryptType Encrypt() const = 0;
  virtual CommandType Command() const = 0;
  virtual bool Verify() const = 0;
  virtual unsigned long DataSize() const = 0;
  virtual unsigned long OriginalSize() const = 0;
  virtual unsigned long PacketSize() const = 0;
 };

 class IProtocol {
 public:
 };

 class IConfig {
 public:
  virtual const std::string& IPAddrV4() const = 0;
  virtual void IPAddrV4(const std::string&) = 0;
  virtual const std::string& IPAddrV6() const = 0;
  virtual void IPAddrV6(const std::string&) = 0;
  virtual const unsigned int& Port() const = 0;
  virtual void Port(const unsigned int&) = 0;
  virtual const EnServerType& ServerType() const = 0;
  virtual void ServerType(const EnServerType&) = 0;
  virtual const std::string& PipeName() const = 0;
  virtual void PipeName(const std::string&) = 0;
 };

 class ISession {
 public:
  virtual bool Ready() const = 0;
  virtual SessionStatus Status() const = 0;
  virtual const EnSessionType& SessionType() const = 0;
 };

 class IClient : public shared::InterfaceDll<IClient> {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual ConnectionStatus ConnStatus() const = 0;
  virtual void Release() const = 0;
 };

 class IServer : public shared::InterfaceDll<IServer> {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual ServerStatus Status() const = 0;
  virtual void Release() const = 0;
 };

}///namespace libuv


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 03:56:34 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__46FA06EA_358C_4885_AF2C_24AFCB15616F__


