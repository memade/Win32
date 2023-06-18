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
#if !defined(__60D7D69B_E61A_487A_B160_4B4D5B8846C2__)
#define __60D7D69B_E61A_487A_B160_4B4D5B8846C2__

#include <dllinterface.hpp>

namespace curl {
 using TypeIdentify = unsigned long long;
 using TypeRequestType = unsigned long;
 using TypeResultCodeType = long;

 enum class ResultCode : TypeResultCodeType {
  UNKNOWN = -1,
  OK = 0,
  UNSUPPORTED_PROTOCOL,    /* 1 */
  FAILED_INIT,             /* 2 */
  URL_MALFORMAT,           /* 3 */
  NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
                                    7.17.0, reused in April 2011 for 7.21.5] */
  COULDNT_RESOLVE_PROXY,   /* 5 */
  COULDNT_RESOLVE_HOST,    /* 6 */
  COULDNT_CONNECT,         /* 7 */
  WEIRD_SERVER_REPLY,      /* 8 */
  REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                    due to lack of access - when login fails
                                    this is not returned. */
  FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
                                    7.15.4, reused in Dec 2011 for 7.24.0]*/
  FTP_WEIRD_PASS_REPLY,    /* 11 */
  FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
                                    [was obsoleted in August 2007 for 7.17.0,
                                    reused in Dec 2011 for 7.24.0]*/
  FTP_WEIRD_PASV_REPLY,    /* 13 */
  FTP_WEIRD_227_FORMAT,    /* 14 */
  FTP_CANT_GET_HOST,       /* 15 */
  HTTP2,                   /* 16 - A problem in the http2 framing layer.
                                    [was obsoleted in August 2007 for 7.17.0,
                                    reused in July 2014 for 7.38.0] */
  FTP_COULDNT_SET_TYPE,    /* 17 */
  PARTIAL_FILE,            /* 18 */
  FTP_COULDNT_RETR_FILE,   /* 19 */
  OBSOLETE20,              /* 20 - NOT USED */
  QUOTE_ERROR,             /* 21 - quote command failure */
  HTTP_RETURNED_ERROR,     /* 22 */
  WRITE_ERROR,             /* 23 */
  OBSOLETE24,              /* 24 - NOT USED */
  UPLOAD_FAILED,           /* 25 - failed upload "command" */
  READ_ERROR,              /* 26 - couldn't open/read from file */
  OUT_OF_MEMORY,           /* 27 */
  OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
  OBSOLETE29,              /* 29 - NOT USED */
  FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
  FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
  OBSOLETE32,              /* 32 - NOT USED */
  RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
  HTTP_POST_ERROR,         /* 34 */
  SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
  BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
  FILE_COULDNT_READ_FILE,  /* 37 */
  LDAP_CANNOT_BIND,        /* 38 */
  LDAP_SEARCH_FAILED,      /* 39 */
  OBSOLETE40,              /* 40 - NOT USED */
  FUNCTION_NOT_FOUND,      /* 41 - NOT USED starting with 7.53.0 */
  ABORTED_BY_CALLBACK,     /* 42 */
  BAD_FUNCTION_ARGUMENT,   /* 43 */
  OBSOLETE44,              /* 44 - NOT USED */
  INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
  OBSOLETE46,              /* 46 - NOT USED */
  TOO_MANY_REDIRECTS,      /* 47 - catch endless re-direct loops */
  UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
  SETOPT_OPTION_SYNTAX,    /* 49 - Malformed setopt option */
  OBSOLETE50,              /* 50 - NOT USED */
  OBSOLETE51,              /* 51 - NOT USED */
  GOT_NOTHING,             /* 52 - when this is a specific error */
  SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
  SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                    default */
  SEND_ERROR,              /* 55 - failed sending network data */
  RECV_ERROR,              /* 56 - failure in receiving network data */
  OBSOLETE57,              /* 57 - NOT IN USE */
  SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
  SSL_CIPHER,              /* 59 - couldn't use specified cipher */
  PEER_FAILED_VERIFICATION, /* 60 - peer's certificate or fingerprint
                                     wasn't verified fine */
  BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
  OBSOLETE62,              /* 62 - NOT IN USE since 7.82.0 */
  FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
  USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
  SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                    that failed */
  SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
  LOGIN_DENIED,            /* 67 - user, password or similar was not
                                    accepted and we failed to login */
  TFTP_NOTFOUND,           /* 68 - file not found on server */
  TFTP_PERM,               /* 69 - permission problem on server */
  REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
  TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
  TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
  REMOTE_FILE_EXISTS,      /* 73 - File already exists */
  TFTP_NOSUCHUSER,         /* 74 - No such user */
  OBSOLETE75,              /* 75 - NOT IN USE since 7.82.0 */
  OBSOLETE76,              /* 76 - NOT IN USE since 7.82.0 */
  SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                    or wrong format */
  REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
  SSH,                     /* 79 - error from the SSH layer, somewhat
                                    generic so the error message will be of
                                    interest when this has happened */

  SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                    connection */
  AGAIN,                   /* 81 - socket is not ready for send/recv,
                                    wait till it's ready and try again (Added
                                    in 7.18.2) */
  SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                    wrong format (Added in 7.19.0) */
  SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                    7.19.0) */
  FTP_PRET_FAILED,         /* 84 - a PRET command failed */
  RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
  RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
  FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
  CHUNK_FAILED,            /* 88 - chunk callback reported error */
  NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the
                                    session will be queued */
  SSL_PINNEDPUBKEYNOTMATCH, /* 90 - specified pinned public key did not
                                     match */
  SSL_INVALIDCERTSTATUS,   /* 91 - invalid certificate status */
  HTTP2_STREAM,            /* 92 - stream error in HTTP/2 framing layer
                                    */
  RECURSIVE_API_CALL,      /* 93 - an api function was called from
                                    inside a callback */
  AUTH_ERROR,              /* 94 - an authentication function returned an
                                    error */
  HTTP3,                   /* 95 - An HTTP/3 layer problem */
  QUIC_CONNECT_ERROR,      /* 96 - QUIC connection error */
  PROXY,                   /* 97 - proxy handshake error */
  SSL_CLIENTCERT,          /* 98 - client-side certificate required */
  UNRECOVERABLE_POLL,      /* 99 - poll/select returned fatal error */
  LAST /* never use! */
 };

 enum class RequestType : TypeRequestType {
  UNKNOWN = 0x0000,
  GET = 0x00000100,
  POST = 0x00000200,
  DOWN = 0x00000300,
  UPLOAD = 0x00000400,

  BASE = 0x00000FFF,
  HEAD = 0x00001000,
 };

 enum class RequestAction : long {
  UNKNOWN = 0x0000,
  PREPARTION = 0x0010,
  START = 0x0020,
  STOP = 0x0030,
  PAUSE = 0x0040,
  REMOVE = 0x0050,
 };

 enum class RequestStatus : long {
  UNKNOWN = 0x0000,
  UNREADY = UNKNOWN,
  PREPARING = 0x0011,
  PREPARAED = 0x0012,
  READY = PREPARAED,
  STARTING = 0x0021,
  RUNNING = 0x0022,
  ACTIVATE = RUNNING,
  STARTED = 0x0023,
  STOPPING = 0x0031,
  STOPPED = 0x0032,
  FINISHED = STOPPED,
  PAUSING = 0x0041,
  PAUSED = 0x0042,
  REMOVING = 0x51,
  REMOVED = 0x52,
  SUCCESS = 0xFFFF,
  FAILED = 0 - SUCCESS,
 };


 class IConfig {
 public:
  virtual void StatusUpdateInterval(const std::uint64_t&) = 0;
  virtual void ProcessExecuteInterval(const std::uint64_t&) = 0;
  virtual void MaximumNumberOfTasks(const std::uint8_t&) = 0;
  // Enables or disables pre-request header information before an HTTP request.
  // virtual void EnableDefaultPrerequestedHeader(const bool&) = 0;
  /*
  @brief Enables or disables pre-requested header information before an HTTP request.
  This method allows the user to enable or disable the inclusion of pre-requested header information
  in an HTTP request. When enabled, the default pre-requested headers will be included in the request,
  and when disabled, these headers will be omitted.
  @param enable Flag indicating whether to enable or disable the default pre-requested headers.
  rust
  Copy code
     - true: Default pre-requested headers will be included in the request.
  sql
  Copy code
     - false: Default pre-requested headers will be omitted from the request.
  @note Enabling pre-requested headers can provide additional information to the server about the client
  Copy code
    or the nature of the request being made.
  @note The implementation of this method is expected to be overridden in derived classes.
  csharp
  Copy code
    This pure virtual function serves as an interface declaration.
  @see SetDefaultPrerequestedHeaders() for setting the actual pre-requested headers to be included.
  */
  virtual void EnableDefaultPrerequestedHeader(const bool&) = 0;
  virtual void EnableSSLSupport(const bool&) = 0;
  virtual bool EnableSSLSupport() const = 0;
  virtual void DownCacheDirectory(const char*) = 0;
 };

 typedef class IRequest {
 protected:
  using tfProgressCb = std::function<void()>;
  using tfCBufferRouteCb = std::function<void(const char*, const size_t&)>;
  using tfWriteStreamCb = std::function<size_t(char*, size_t, size_t)>;
 public:
  virtual TypeIdentify Identify() const = 0;
  virtual const char* Url() const = 0;
  virtual const char* UrlHost() const = 0;
  virtual const char* UrlPath() const = 0;
  virtual const char* UrlSchema() const = 0;
  virtual const char* UrlFileName() const = 0;
  virtual const char* UrlFileFormat() const = 0;
  virtual const char* WriteCachePath() const = 0;
  virtual const char* CacheFileNameFinal() const = 0;
  virtual const char* CacheFileNameTemp() const = 0;
  virtual void Type(const TypeRequestType&) = 0;
  virtual RequestType Type() const = 0;
  virtual void Header(const bool&) = 0;
  virtual const RequestStatus& Status() const = 0;
  virtual const ResultCode& Reason() const = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Pause() = 0;
  virtual void Remove() = 0;
  virtual bool Success() const = 0;
  virtual void RegisterProgressCb(const tfProgressCb&) = 0;
  virtual void RegisterWriteStreamCb(const tfWriteStreamCb&) = 0;
  virtual void WriteCachePath(const char*) = 0;
  virtual bool EnableWriteCache(const bool&) = 0;
  virtual bool EnableWriteCache() const = 0;
  virtual void ConnectTimeout(const std::uint32_t&) = 0;
  virtual void Timeout(const std::uint32_t&) = 0;
  virtual size_t ContentLength() const = 0;
  virtual void Body(const tfCBufferRouteCb&) const = 0;
  virtual void Verbose(const bool&) = 0;
 }IResponse;


 class ICurl : public shared::InterfaceDll<ICurl> {
 protected:
  using tfRequestDoneCb = std::function<void(const IRequest*)>;
  using tfRequestBeginCb = std::function<void(const IRequest*)>;
  using tfRequestRemoveCb = std::function<void(const IRequest*)>;
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual IRequest* CreateRequest(const char* url) = 0;
  virtual bool DestroyRequest(const IRequest*&) = 0;
  virtual void RegisterRequestDoneCb(const tfRequestDoneCb&) = 0;
  virtual void RegisterRequestBeginCb(const tfRequestDoneCb&) = 0;
  virtual void RegisterRequestRemoveCb(const tfRequestRemoveCb&) = 0;
  virtual void Release() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
 };


}///namespace curl

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 05 Jun 2023 13:55:28 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__60D7D69B_E61A_487A_B160_4B4D5B8846C2__



