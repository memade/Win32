#if !defined(__6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__)
#define __6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__

namespace local {
#pragma pack(push,1)
 // Define a C++ struct called "PacketHeader", which inherits from the "IPacketHeader" interface
 typedef struct tagPacketHeader : public IPacketHeader {
  // Packet header logo
  unsigned long header_logo;
  // Server identify
  unsigned long long server_identify;
  // Original data size
  unsigned long original_size;
  // Real-time (actual) data size
  unsigned long data_size;
  // Total size of the packet frame
  unsigned long packet_size;
  // Command code
  TypeCommandType command_code;
  // ZIP compression type
  TypeZipType zip_type;
  // Packet encryption type
  TypeEncryptType encryption_type;
  // Packet footer logo
  unsigned long footer_logo;
  // Character array used to encapsulate the data
  char data[1];

  // Constructors
  tagPacketHeader();
  tagPacketHeader(const CommandType&);

  // Member functions
  // Verify the packet header
  bool Verify() const override final;
  // Return the ZIP compression type
  ZipType Zip() const override final;
  // Return the packet encryption type
  EncryptType Encrypt() const override final;
  // Return the command code
  CommandType Command() const override final;
  // Return the real-time data size
  unsigned long DataSize() const override final;
  // Return the original data size
  unsigned long OriginalSize() const override final;
  // Return the packet totoal size
  unsigned long PacketSize() const override final;
 }PacketHeader, HEAD,* PHEAD;
#pragma pack(pop)



 class Protocol final : public IProtocol {
 public:
  Protocol();
  virtual ~Protocol();
 public:
  static std::string MakeStream(const HEAD&, const std::string&);
  static bool UnMakeStream(const std::string& input, HEAD&, std::string& output);
  static void uv_async_cb(uv_async_t* async_handle);
  static void uv_close_loop(void* uv_handle_loop = nullptr);
  static void uv_close_default_loop();
  static TypeIdentify make_sock_session_identify(const SessionType&, const struct sockaddr&);
  static bool unmake_sock_session_identify(const TypeIdentify&, SessionType&, struct sockaddr&);
  static bool unmake_sock_session_identify(const TypeIdentify&, SessionType&, std::string& ip,u_short& port);
  static std::string unmake_sock_session_identify(const TypeIdentify&);
  static TypeIdentify make_pipe_session_identify(const SessionType&/*unsigned long*/ type);
  static bool unmake_pipe_session_identify(const TypeIdentify&,SessionType&,time_t&);
  static bool parser_ipaddr(const std::string& address, std::string& ip, u_short& port);
  static bool parser_ipaddr(const sockaddr_storage* in_addr, std::string& out_ip, u_short& out_port, const IPType& ipv = IPType::IPV4);
 };




 extern const size_t PACKET_HEAD_SIZE;
 extern const size_t PACKET_COMPRESSION_STANDARD_SIZE;
 extern const std::uint64_t TIMEOUT_HEARBEAT_MS;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 25 Apr 2023 01:18:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__