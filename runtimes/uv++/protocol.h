#if !defined(__291A2E20_BCB0_4E74_985A_ED27CF6A5528__)
#define __291A2E20_BCB0_4E74_985A_ED27CF6A5528__

namespace local {
#define SESSION_PTR(uv_handle) reinterpret_cast<Session*>(uv_handle->data) 
#define USERDATA_PTR(p) SESSION_PTR(p)

#pragma pack(push,1)

 // Define a C++ struct called "PacketHeader", which inherits from the "IPacketHeader" interface
 typedef struct tagPacketHeader {
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
  bool Verify() const;
  // Return the ZIP compression type
  ZipType Zip() const;
  // Return the packet encryption type
  EncryptType Encrypt() const;
  // Return the command code
  CommandType Command() const;
  // Return the real-time data size
  unsigned long DataSize() const;
  // Return the original data size
  unsigned long OriginalSize() const;
  // Return the packet totoal size
  unsigned long PacketSize() const;
 }PacketHeader, HEAD, * PHEAD;
#pragma pack(pop)



 class Protocol final {
 public:
  Protocol();
  virtual ~Protocol();
 public:
  static std::string MakeStream(const HEAD&, const std::string&);
  static bool UnMakeStream(const std::string& input, HEAD&, std::string& output);
  static bool parser_ipaddr(const std::string& address, std::string& ip, u_short& port);
  static bool parser_ipaddr(const sockaddr* in_addr, char*);
  static bool parser_ipaddr(const sockaddr* in_addr, std::string&);
  static bool parser_ipaddr(const sockaddr_storage* in_addr, std::string& out_ip, u_short& out_port, const IPPROTO& ipv = IPPROTO::IPPROTO_IPV4);
 public:
  static void uv_alloc_cb(uv_handle_t* handle,
   size_t suggested_size,
   uv_buf_t* buf);
  static void uv_recv_cb(uv_stream_t* stream,
   ssize_t nread,
   const uv_buf_t* buf);
  static void uv_write_cb(uv_write_t* req, int status);
  static void uv_close_cb(uv_handle_t* handle);
  static void walk_close_handle_cb(uv_handle_t* handle, void* arg);
 };




 extern const size_t PACKET_HEAD_SIZE;
 extern const size_t PACKET_COMPRESSION_STANDARD_SIZE;
 extern const std::uint64_t TIMEOUT_HEARBEAT_MS;



}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 14:28:32 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__291A2E20_BCB0_4E74_985A_ED27CF6A5528__