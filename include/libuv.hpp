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
 using TypeErrnoType = long;

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

 enum class SystemErrorno : TypeErrnoType {
  E_UNKNOWN = -0xFFFF,
  E_STREAM_PARSING = E_UNKNOWN + 1,
  E_STREAM_RECV = E_STREAM_PARSING,
  E_READ = E_STREAM_PARSING,
  E_STREAM_SEND = E_UNKNOWN + 2,
  E_WRITE = E_STREAM_SEND,
  E_SYSTEM_CORE_INIT = E_UNKNOWN + 3,
  E_SERVER_SESSION_INIT = E_UNKNOWN + 4,
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
  virtual unsigned long long KeepAliveTimeMS() const = 0;
  virtual void KeepAliveTimeMS(const unsigned long long&) = 0;
  virtual unsigned long long ClientReconnectionIntervalMS() const = 0;
  virtual void ClientReconnectionIntervalMS(const unsigned long long&) = 0;
 };

 class ISession {
 public:
  virtual void Route(void*) = 0;
  virtual void* Route() const = 0;
  virtual bool Read(const char*, const size_t&) = 0;
  virtual bool Write(const CommandType&, const std::string&) = 0;
  virtual const SessionStatus& Status() const = 0;
  virtual void Address(const std::string&) = 0;
  virtual void ForceClose() = 0;
  virtual const std::string& Address() const = 0;
  virtual void ServerTypeSet(const ServerType&) = 0;
  virtual const ServerType& ServerTypeGet() const = 0;
  virtual void SessionTypeSet(const SessionType&) = 0;
  virtual const SessionType& SessionTypeGet() const = 0;
  virtual unsigned long long CreateTimeMS() const = 0;
 };

 class ICallback {
 protected:
  using tfOnServerReadyCb = std::function<void(void)>;
  using tfOnForceCloseCb = std::function<void(const ISession*)>;
  using tfOnAcceptCb = std::function<void(const ISession*, const bool& success)>;
  using tfOnHookSessionCreateCb = std::function<void(ISession*)>;
  using tfOnHookSessionDestoryCb = std::function<void(ISession*)>;
  using tfOnMessageCb = std::function<void(const ISession*, const CommandType&, const std::string&)>;
  using tfOnConnectionCb = std::function<void(const ISession*)>;
  using tfOnDisconnectionCb = std::function<void(const ISession*)>;
  using tfOnSystemMessageCb = std::function<void(const ISession*, const SystemErrorno&, const std::string&)>;
  using tfOnReceiveReplyCb = std::function<void(const ISession*, \
   const CommandType& cmd_receive, const std::string& message_receive,
   CommandType& cmd_reply,
   std::string& message_reply)>;
  using tfOnHelloCb = std::function<void(const ISession*, const std::string& message_receive, std::string& message_reply)>;
  using tfOnWelcomeCb = tfOnHelloCb;
  using tfOnHookWelcomeSendCb = std::function<void(const ISession*, std::string&)>;
  using tfOnHookWriteCb = std::function<void(const ISession*, std::string&)>;
  using tfOnSystemExitCb = std::function<void(void)>;
  using tfOnHookSystemExitCb = std::function<void(bool& action/*Yes|No*/)>;
  using tfOnHookConnectionCb = std::function<void(std::string& address)>;
  using tfOnHookServerAddressCb = std::function<void(std::string& address)>;
  using tfOnServerSessionTimeoutCb = std::function<void(const ISession*, const std::time_t&)>;
  using tfOnServerKeepAliveCb = std::function<void(const ISession*, const std::string& message)>;
  using tfOnServerSessionReadyCb = std::function<void(const ISession*)>;
  using tfOnHookKeepAliveSendCb = std::function<void(const ISession*, std::string&)>;
  using tfOnKeepAliveCb = std::function<void(const ISession*, const std::string&, std::string&)>;
 protected:
  tfOnKeepAliveCb m_OnKeepAliveCb = nullptr;
  tfOnHookKeepAliveSendCb m_OnHookKeepAliveSendCb = nullptr;
  tfOnHelloCb m_OnHelloCb = nullptr;
  tfOnWelcomeCb m_OnWelcomeCb = nullptr;
  tfOnHookWelcomeSendCb m_OnHookWelcomeSendCb = nullptr;
  tfOnServerSessionReadyCb m_OnServerSessionReadyCb = nullptr;
  tfOnAcceptCb m_OnAcceptCb = nullptr;
  tfOnMessageCb m_OnMessageCb = nullptr;
  tfOnSystemExitCb m_OnSystemExitCb = nullptr;
  tfOnHookWriteCb m_OnHookWriteCb = nullptr;
  tfOnForceCloseCb m_OnForceCloseCb = nullptr;
  tfOnConnectionCb m_OnConnectionCb = nullptr;
  tfOnServerReadyCb m_OnServerReadyCb = nullptr;
  tfOnReceiveReplyCb m_OnReceiveReplyCb = nullptr;
  tfOnDisconnectionCb m_OnDisconnectionCb = nullptr;
  tfOnServerKeepAliveCb m_OnServerKeepAliveCb = nullptr;
  tfOnHookSystemExitCb m_OnHookSystemExitCb = nullptr;
  tfOnSystemMessageCb m_OnSystemMessageCb = nullptr;
  tfOnHookConnectionCb m_OnHookConnectionCb = nullptr;
  tfOnHookServerAddressCb m_OnHookServerAddressCb = nullptr;
  tfOnServerSessionTimeoutCb m_OnServerSessionTimeoutCb = nullptr;
  tfOnHookSessionCreateCb m_OnHookSessionCreateCb = nullptr;
  tfOnHookSessionDestoryCb m_OnHookSessionDestoryCb = nullptr;
 public:
  virtual void RegisterOnKeepAliveCb(const tfOnKeepAliveCb& cb) { m_OnKeepAliveCb = cb; }
  virtual void RegisterOnHookKeepAliveSendCb(const tfOnHookKeepAliveSendCb& cb) { m_OnHookKeepAliveSendCb = cb; }
  virtual void RegisterOnServerSessionReadyCb(const tfOnServerSessionReadyCb& cb) { m_OnServerSessionReadyCb = cb; }
  virtual void RegisterOnHookWelcomeSendCb(const tfOnHookWelcomeSendCb& cb) { m_OnHookWelcomeSendCb = cb; }
  virtual void RegisterOnHelloCb(const tfOnHelloCb& cb) { m_OnHelloCb = cb; }
  virtual void RegisterOnWelcomeCb(const tfOnWelcomeCb& cb) { m_OnWelcomeCb = cb; }
  virtual void RegisterOnServerKeepAliveCb(const tfOnServerKeepAliveCb& cb) { m_OnServerKeepAliveCb = cb; }
  virtual void RegisterOnServerSessionTimeoutCb(const tfOnServerSessionTimeoutCb& cb) { m_OnServerSessionTimeoutCb = cb; }
  virtual void RegisterOnServerReadyCb(const tfOnServerReadyCb& cb) { m_OnServerReadyCb = cb; }
  virtual void RegisterOnSystemExitCb(const tfOnSystemExitCb& cb) { m_OnSystemExitCb = cb; }
  virtual void RegisterOnHookSystemExitCb(const tfOnHookSystemExitCb& cb) { m_OnHookSystemExitCb = cb; }
  virtual void RegisterOnForceClose(const tfOnForceCloseCb& cb) { m_OnForceCloseCb = cb; }
  virtual void RegisterOnAcceptCb(const tfOnAcceptCb& cb) { m_OnAcceptCb = cb; }
  virtual void RegisterOnMessageCb(const tfOnMessageCb& cb) { m_OnMessageCb = cb; }
  virtual void RegisterOnConnectionCb(const tfOnConnectionCb& cb) { m_OnConnectionCb = cb; }
  virtual void RegisterOnDisconnectionCb(const tfOnDisconnectionCb& cb) { m_OnDisconnectionCb = cb; }
  virtual void RegisterOnSystemMessageCb(const tfOnSystemMessageCb& cb) { m_OnSystemMessageCb = cb; }
  virtual void RegisterOnReceiveReplyCb(const tfOnReceiveReplyCb& cb) { m_OnReceiveReplyCb = cb; }
  virtual void RegisterOnHookWriteCb(const tfOnHookWriteCb& cb) { m_OnHookWriteCb = cb; }
  virtual void RegisterOnHookSessionCreateCb(const tfOnHookSessionCreateCb& cb) { m_OnHookSessionCreateCb = cb; }
  virtual void RegisterOnHookSessionDestoryCb(const tfOnHookSessionDestoryCb& cb) { m_OnHookSessionDestoryCb = cb; }
  virtual void RegisterOnHookConnectionCb(const tfOnHookConnectionCb& cb) { m_OnHookConnectionCb = cb; }
  virtual void RegisterOnHookServerAddressCb(const tfOnHookServerAddressCb& cb) { m_OnHookServerAddressCb = cb; }
 protected:
  virtual void OnKeepAlive(const ISession* session, const std::string& message, std::string& message_reply) {
   if (m_OnKeepAliveCb) {
    m_OnKeepAliveCb(session, message, message_reply);
   }
  }
  virtual void OnHookKeepAliveSend(const ISession* session, std::string& message) {
   if (m_OnHookKeepAliveSendCb) {
    m_OnHookKeepAliveSendCb(session, message);
   }
  }
  virtual void OnServerSessionReady(const ISession* session) {
   if (m_OnServerSessionReadyCb) {
    m_OnServerSessionReadyCb(session);
   }
  }
  virtual void OnHookWelcomeSend(const ISession* session, std::string& message) {
   if (m_OnHookWelcomeSendCb) {
    m_OnHookWelcomeSendCb(session, message);
   }
  }
  virtual void OnHello(const ISession* session, const std::string& message_receive, std::string& message_reply) {
   if (m_OnHelloCb) {
    m_OnHelloCb(session, message_receive, message_reply);
   }
  }
  virtual void OnWelcome(const ISession* session, const std::string& message_receive, std::string& message_reply) {
   if (m_OnWelcomeCb) {
    m_OnWelcomeCb(session, message_receive, message_reply);
   }
  }
  virtual void OnServerKeepAlive(const ISession* session, const std::string& message) {
   if (m_OnServerKeepAliveCb) {
    m_OnServerKeepAliveCb(session, message);
   }
  }
  virtual void OnServerSessionTimeout(const ISession* session, const std::time_t& timeout_ms) {
   if (m_OnServerSessionTimeoutCb) {
    m_OnServerSessionTimeoutCb(session, timeout_ms);
   }
  }
  virtual void OnServerReady() {
   if (m_OnServerReadyCb) {
    m_OnServerReadyCb();
   }
  }
  virtual void OnHookServerAddress(std::string& address) {
   if (m_OnHookServerAddressCb) {
    m_OnHookServerAddressCb(address);
   }
  }
  virtual void OnHookConnection(std::string& address) {
   if (m_OnHookConnectionCb) {
    m_OnHookConnectionCb(address);
   }
  }
  virtual void OnSystemExit() {
   if (m_OnSystemExitCb) {
    m_OnSystemExitCb();
   }
  }
  virtual void OnHookSystemExit(bool& action) {
   if (m_OnHookSystemExitCb) {
    m_OnHookSystemExitCb(action);
   }
  }
  virtual void OnAccept(const ISession* session, const bool& success) {
   if (m_OnAcceptCb) { m_OnAcceptCb(session, success); }
  }
  virtual void OnForceClose(const ISession* session) {
   if (m_OnForceCloseCb) { m_OnForceCloseCb(session); }
  }
  virtual void OnConnection(const ISession* session) {
   if (m_OnConnectionCb) { m_OnConnectionCb(session); }
  }
  virtual void OnDisconnection(const ISession* session) {
   if (m_OnDisconnectionCb) { m_OnDisconnectionCb(session); }
  }
  virtual void OnHookSessionCreate(ISession* session) {
   if (m_OnHookSessionCreateCb) { m_OnHookSessionCreateCb(session); }
  }
  virtual void OnHookSessionDestory(ISession* session) {
   if (m_OnHookSessionDestoryCb) { m_OnHookSessionDestoryCb(session); }
  }
  virtual void OnMessage(const ISession* session, const CommandType& cmd, const std::string& message) {
   if (m_OnMessageCb) { m_OnMessageCb(session, cmd, message); }
  }
  virtual void OnSystemMessage(const ISession* session, const SystemErrorno& status, const std::string& message) {
   if (m_OnSystemMessageCb) { m_OnSystemMessageCb(session, status, message); }
  }
  virtual void OnReceiveReply(const ISession* session,
   const CommandType& cmd_receive, const std::string& message_receive, CommandType& cmd_reply,
   std::string& message_reply) {
   if (m_OnReceiveReplyCb) {
    m_OnReceiveReplyCb(session, cmd_receive, message_receive, cmd_reply, message_reply);
   }
  }
  virtual void OnHookWrite(const ISession* session, std::string& message) {
   if (m_OnHookWriteCb) {
    m_OnHookWriteCb(session, message);
   }
  }
 };

 class IClient
  : public shared::InterfaceDll<IClient>
  , public ICallback {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual SessionStatus Status() const = 0;
  virtual void Release() const = 0;
 };

 class IServer
  : public shared::InterfaceDll<IServer>
  , public ICallback {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual ServerStatus Status() const = 0;
  virtual void Release() const = 0;
  virtual unsigned long SessionCount() const = 0;
 };

}///namespace libuv


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 03:56:34 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__46FA06EA_358C_4885_AF2C_24AFCB15616F__


