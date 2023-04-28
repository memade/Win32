#if !defined(__6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__)
#define __6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__

namespace local {
#pragma pack(push,1)
 // Define a C++ struct called "PacketHeader", which inherits from the "IPacketHeader" interface
 struct PacketHeader : public IPacketHeader {
  // Packet header logo
  unsigned long header_logo;
  // Original data size
  unsigned long original_size;
  // Real-time (actual) data size
  unsigned long data_size;
  // Total size of the packet frame
  unsigned long packet_size;
  // Command code
  unsigned long command_code;
  // ZIP compression type
  int zip_type;
  // Packet encryption type
  int encryption_type;
  // Packet footer logo
  unsigned long footer_logo;
  // Character array used to encapsulate the data
  char data[1];

  // Constructors
  PacketHeader();
  PacketHeader(const CommandType&);

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
 };
 // Define shorthand names for the struct
 typedef PacketHeader Head;
 typedef PacketHeader HEAD;
 typedef PacketHeader* PHEAD;
 typedef PacketHeader PACKETHEAD;
 typedef PacketHeader* PPACKETHEAD;

 typedef struct {
  uv_write_t req;
  uv_buf_t buf;
  void* handle;
 } write_req_t;

 typedef enum {
  Unknown = 0,
  Write = 1,
  Close = 2,
 }async_req_type;

 typedef struct {
  void* data;
  void* host;
  async_req_type req_type;
 } async_req_t;

 typedef struct {
  int status;
  void* caller;
  void* route;
  int connect_action;
 }connect_callback_route_t;
#pragma pack(pop)

 class Protocol final : public IProtocol {
 public:
  Protocol();
  virtual ~Protocol();
 public:
  static std::string MakeStream(const HEAD&, const std::string&);
  static bool UnMakeStream(const std::string& input, HEAD&, std::string& output);
 };




 extern const size_t PACKET_HEAD_SIZE;
 extern const size_t PACKET_COMPRESSION_STANDARD_SIZE;
 extern const std::uint64_t TIMEOUT_HEARBEAT_MS;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 25 Apr 2023 01:18:27 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__6CCB7E2E_3DEB_4F64_A667_EBA870083DC4__