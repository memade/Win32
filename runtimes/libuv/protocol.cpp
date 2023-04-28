#include "stdafx.h"

namespace local {

 PacketHeader::PacketHeader() {
  memset(this, 0x00, sizeof(*this));
  header_logo = 0xFAC9C2D0;
  footer_logo = 0xB4B4AAC1;
 }
 PacketHeader::PacketHeader(const CommandType& input_cmd) {
  memset(this, 0x00, sizeof(*this));
  header_logo = 0xFAC9C2D0;
  footer_logo = 0xB4B4AAC1;
  command_code = static_cast<decltype(command_code)>(input_cmd);
 }
 bool PacketHeader::Verify() const {
  return header_logo == 0xFAC9C2D0 && footer_logo == 0xB4B4AAC1;
 }
 ZipType PacketHeader::Zip() const {
  return ZipType(zip_type);
 }
 EncryptType PacketHeader::Encrypt() const {
  return EncryptType(encryption_type);
 }
 CommandType PacketHeader::Command() const {
  return CommandType(command_code);
 }
 unsigned long PacketHeader::DataSize() const {
  return data_size;
 }
 unsigned long PacketHeader::OriginalSize() const {
  return original_size;
 }
 unsigned long PacketHeader::PacketSize() const {
  return packet_size;
 }


 Protocol::Protocol() {

 }

 Protocol::~Protocol() {

 }


 std::string Protocol::MakeStream(const HEAD& input_head, const std::string& input_data) {
  std::string result;
  do {
   result.append((char*)&input_head, PACKET_HEAD_SIZE);
   PHEAD pHead = reinterpret_cast<PHEAD>(result.data());
   pHead->zip_type = static_cast<decltype(pHead->zip_type)>(ZipType::UNDEFINE);
   pHead->encryption_type = static_cast<decltype(pHead->encryption_type)>(EncryptType::UNDEFINE);
   pHead->data_size = input_data.size();
   pHead->original_size = pHead->data_size;
   do {
    if (pHead->original_size <= PACKET_COMPRESSION_STANDARD_SIZE)
     break;
    std::string zip;
    if (!shared::Zip::zipCompress(input_data, zip) || zip.empty())
     break;
    pHead->zip_type = static_cast<decltype(pHead->zip_type)>(ZipType::ZIP);
    pHead->data_size = zip.size();
    result.pop_back();
    result.append(zip);
    break;
   } while (0);
   if (ZipType(pHead->zip_type) != ZipType::UNDEFINE)
    break;
   if (input_data.empty())
    break;
   result.pop_back();
   result.append(input_data);
  } while (0);
  if (!result.empty())
   ((PHEAD)result.data())->packet_size = result.size();
  return result;
 }
 bool Protocol::UnMakeStream(const std::string& input_data, HEAD& head, std::string& output_data) {
  bool result = false;
  memset(&head, 0x00, PACKET_HEAD_SIZE);
  output_data.clear();
  do {
   PHEAD pHead = (PHEAD)input_data.data();
   if (!pHead->Verify())
    break;
   memcpy(&head, pHead, PACKET_HEAD_SIZE);
   switch (ZipType(pHead->Zip())) {
   case ZipType::UNDEFINE: {
    if (!pHead->DataSize())
     break;
    output_data.append(pHead->data, pHead->DataSize());
   }break;
   case ZipType::ZIP: {
    shared::Zip::zipUnCompress(pHead->data, pHead->OriginalSize(), output_data);
   }break;
   case ZipType::GZIP: {
    shared::Zip::gzipUnCompress(pHead->data, output_data);
   }break;
   default:
    break;
   }
   result = true;
  } while (0);
  return result;
 }


 const std::uint64_t TIMEOUT_HEARBEAT_MS = 30000;
 const size_t PACKET_HEAD_SIZE = sizeof(HEAD);
 const size_t PACKET_COMPRESSION_STANDARD_SIZE = 512;
}///namespace local
