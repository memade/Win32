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
 using TypeSessionIDType = unsigned long;
 using TypeSessionType = unsigned short;
 using TypeIPType = unsigned char;
 using TypeStreamType = unsigned char;
 using TypeZipType = unsigned char;
 using TypeConnectionStatus = unsigned char;
 using TypeCommandType = unsigned long;
 using TypeEncryptType = unsigned char;


 using tf_api_object_init = void* (__stdcall*)(const void*, unsigned long);
 using tf_api_object_uninit = void(__stdcall*)(void);


 enum class IPType : TypeIPType {
  Unknown = 0x0,
  IPV4 = 0x1,
  IPV6 = 0x2,
 };

 enum class SessionType : TypeSessionType {
  Unknown = 0x00,
  TCP_SESSION_CLIENT = 0x11,
  UDP_SESSION_CLIENT = 0x12,
  IPC_SESSION_CLIENT = 0x13,
  TCP_SESSION_SERVER = 0x21,
  UDP_SESSION_SERVER = 0x22,
  IPC_SESSION_SERVER = 0x23,
 };

 // Enum for different connection status types with integer values
 typedef enum class ConnectionStatus : TypeConnectionStatus {
  Unknown = 0x0,

  Unready = Unknown,//!@ opening or initialize failed.

  InPreparation = 0x1,
  Ready = 0x2, //!@ Connection is ready

  ForceClose = 0x3, //!@ Close command has been initiated

  Closing = 0x4, //!@ Connection is being closed
  Stopping = Closing,
  Closed = 0x5, //!@ Connection is closed and not opened yet
  Stopped = Closed,
  Disconnected = Closed, //!@ Connection is completed with failure

  Opening = 0x6, //!@ Connection is being opened
  Connecting = Opening, //!@ Connection is being established
  Starting = Opening,
  Opened = 0x7, //!@ Connection is opened successfully
  Started = Opened,
  Connected = Opened, //!@ Connection is completed successfully
  Activate = Opened,

  ConnectReset = 0x8,//!@ reset action.
  ConnectExit = 0x9,//!@ module exit.

  Begin = Unready,
  End = ConnectExit,
 }ClientStatus, ServerStatus, SessionStatus;

 enum class StreamType : TypeStreamType {
  Unknown = 0x0,
  Read = 0x1,
  Write = 0x2,
 };

 enum class ZipType : TypeZipType {
  Unknown = 0x0,
  ZIP = 0x1,
  GZIP = 0x2,
 };

 enum class EncryptType : TypeEncryptType {
  Unknown = 0x0,
  WEMADE = 0x1,
 };

 enum class CommandType : TypeCommandType {
  Unknown = 0x00000,
  Welcome = 0x10000,
  Hello = Welcome,
  HeartBeat = 0x10100,
  KeepAlive = HeartBeat,
  ServerExit = 0x10400,
 };

 static const std::map<SessionType, std::string> SessionTypeString = {
  {SessionType::TCP_SESSION_CLIENT ,"TCP_SESSION_CLIENT"},
  {SessionType::TCP_SESSION_SERVER ,"TCP_SESSION_SERVER"},
  {SessionType::UDP_SESSION_CLIENT ,"UDP_SESSION_CLIENT"},
  {SessionType::UDP_SESSION_SERVER ,"UDP_SESSION_SERVER"},
  {SessionType::IPC_SESSION_CLIENT ,"IPC_SESSION_CLIENT"},
  {SessionType::IPC_SESSION_SERVER ,"IPC_SESSION_SERVER"},
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
  virtual const std::string& PipeName() const = 0;
  virtual void PipeName(const std::string&) = 0;
  virtual std::string Address(const SessionType&) const = 0;
 };

 class ISession {
 public:
  virtual bool Ready() const = 0;
  virtual SessionStatus Status() const = 0;
  virtual const SessionType& Type() const = 0;
  virtual const std::string& Address() const = 0;
  virtual const TypeIdentify& Identify() const = 0;
 };

 class IClient : public shared::InterfaceDll<IClient> {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual ConnectionStatus ConnStatus() const = 0;
  virtual void Release() const = 0;
  virtual unsigned long SessionCount() const = 0;
 };

 class IServer : public shared::InterfaceDll<IServer> {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual const SessionType& Type() const = 0;
  virtual void Release() const = 0;
  virtual unsigned long SessionCount() const = 0;
 };

}///namespace libuv


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 03:56:34 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__46FA06EA_358C_4885_AF2C_24AFCB15616F__


