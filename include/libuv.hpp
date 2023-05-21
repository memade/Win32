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
 using TypeStreamType = unsigned char;
 using TypeZipType = unsigned char;
 using TypeSessionStatus = unsigned char;
 using TypeCommandType = unsigned long;
 using TypeEncryptType = unsigned char;
 using TypeServerType = unsigned long;
 using TypeIPType = unsigned int;
 using TypeSessionType = unsigned long long;

 enum class IPType : TypeIPType {
  UNKNOWN = 0x0,
  IPPROTO_IPV4 = IPPROTO::IPPROTO_IPV4,
  IPPROTO_IPV6 = IPPROTO::IPPROTO_IPV6,
 };

 enum class ServerType : TypeServerType {
  UNKNOWN = 0,
  INITIATOR = 1,
  ACCEPTOR = 2,
 };

 enum class SessionType : TypeSessionType {
  UNKNOWN = 0x00,
  TCP = 0x01,
  UDP = 0x02,
  IPC = 0x03,
  HTTP = 0x04,
 };

 // Enum for different connection status types with integer values
 typedef enum class SessionStatus : TypeSessionStatus {
  UNKNOWN = 0x0,

  UNREADY = UNKNOWN,//!@ opening or initialize failed.

  INPREPARATION = 0x1,
  READY = 0x2, //!@ Connection is ready

  FORCECLOSE = 0x3, //!@ Close command has been initiated

  CLOSING = 0x4, //!@ Connection is being closed
  STOPPING = CLOSING,
  CLOSED = 0x5, //!@ Connection is closed and not opened yet
  STOPPED = CLOSED,
  DISCONNECTED = CLOSED, //!@ Connection is completed with failure

  OPENING = 0x6, //!@ Connection is being opened
  CONNECTING = OPENING, //!@ Connection is being established
  STARTING = OPENING,
  OPENED = 0x7, //!@ Connection is opened successfully
  STARTED = OPENED,
  CONNECTED = OPENED, //!@ Connection is completed successfully
  ACTIVATE = OPENED,


  BEGIN = UNKNOWN,
  END = ACTIVATE,
 }ServerStatus;

 enum class StreamType : TypeStreamType {
  UNKNOWN = 0x0,
  READ = 0x1,
  WRITE = 0x2,
 };

 enum class ZipType : TypeZipType {
  UNKNOWN = 0x0,
  ZIP = 0x1,
  GZIP = 0x2,
 };

 enum class EncryptType : TypeEncryptType {
  UNKNOWN = 0x0,
  WEMADE = 0x1,
 };

 enum class CommandType : TypeCommandType {
  UNKNOWN = 0x00000,
  WELCOME = 0x10000,
  HELLO = WELCOME,
  HEARTBEAT = 0x10100,
  KEEPALIVE = HEARTBEAT,
 };

#if 0
 static const std::map<SessionType, std::string> SessionTypeString = {
  {SessionType::TCP_SESSION_CLIENT ,"TCP_SESSION_CLIENT"},
  {SessionType::TCP_SESSION_SERVER ,"TCP_SESSION_SERVER"},
  {SessionType::UDP_SESSION_CLIENT ,"UDP_SESSION_CLIENT"},
  {SessionType::UDP_SESSION_SERVER ,"UDP_SESSION_SERVER"},
  {SessionType::IPC_SESSION_CLIENT ,"IPC_SESSION_CLIENT"},
  {SessionType::IPC_SESSION_SERVER ,"IPC_SESSION_SERVER"},
 };
#endif

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
  virtual const ServerType& Server() const = 0;
  virtual void Session(const SessionType&) = 0;
  virtual const SessionType& Session() const = 0;
  virtual void IP(const IPType&) = 0;
  virtual const IPType& IP() const = 0;
  virtual void Address(const std::string&) = 0;
  virtual const std::string& Address() const = 0;
  virtual unsigned long long SessionTimeoutMS() const = 0;
  virtual void SessionTimeoutMS(const unsigned long long&) = 0;
  virtual unsigned long long ClientReconnectionIntervalMS() const = 0;
  virtual void ClientReconnectionIntervalMS(const unsigned long long&) = 0;
 };

 class ISession {
 public:
 };

 class IClient : public shared::InterfaceDll<IClient> {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual SessionStatus Status() const = 0;
  virtual void Release() const = 0;
 };

 class IServer : public shared::InterfaceDll<IServer> {
 protected:
  using tfOnAcceptCb = std::function<void(ISession*)>;
  using tfOnSessionAppendBeforeCb = std::function<void(ISession*)>;
  using tfOnSessionRemoveBeforeCb = std::function<void(ISession*)>;
  using tfOnReceiveReply = std::function<void(ISession*, const std::string& receive, std::string& reply)>;
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual ServerStatus Status() const = 0;
  virtual void Release() const = 0;
  virtual unsigned long SessionCount() const = 0;
  virtual void RegisterOnAcceptCb(const tfOnAcceptCb&) = 0;
  virtual void RegisterOnReceiveReply(const tfOnReceiveReply&) = 0;
  virtual void RegisterOnSessionAppendBeforeCb(const tfOnSessionAppendBeforeCb&) = 0;
  virtual void RegisterOnSessionRemoveBeforeCb(const tfOnSessionRemoveBeforeCb&) = 0;
 };

}///namespace libuv


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 03:56:34 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__46FA06EA_358C_4885_AF2C_24AFCB15616F__


