#include "stdafx.h"

namespace local {

 Ssl::Ssl() {
  Init();
 }

 Ssl::~Ssl() {
  UnInit();
 }
 void Ssl::Release() const {
  delete this;
 }
 IConfig* Ssl::ConfigGet() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pConfig;
 }

 void Ssl::Init() {
  m_pConfig = new Config();
 }

 void Ssl::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }

 bool Ssl::EnMD5(const char* input_data, size_t input_data_size, const tfResultDataCb& result_cb) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  std::string result_data;
  do {
   EVP_MD_CTX* ctx = nullptr;
   do {
    if ((ctx = EVP_MD_CTX_new()) == nullptr)
     break;
    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), nullptr))
     break;
    if (1 != EVP_DigestUpdate(ctx, input_data ? input_data : nullptr, input_data_size))
     break;
    result_data.resize(4096/*MD5_DIGEST_LENGTH*/, 0x00);
    unsigned int len = 0;
    if (1 != EVP_DigestFinal_ex(ctx, (std::uint8_t*)&result_data[0], &len)) {
     result_data.clear();
     break;
    }
    result_data.resize(len);
    result = true;
   } while (0);
   if (ctx)
    EVP_MD_CTX_free(ctx);
   if (!result)
    result_data.clear();
  } while (0);
  result_cb(result_data.data(), result_data.size());
  return result;
 }
 bool Ssl::EnBase64(const char* input_data, size_t input_data_size, const bool& newline, const tfResultDataCb& result_cb) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  BIO* bmem = nullptr;
  BIO* b64 = nullptr;
  BUF_MEM* bptr = nullptr;
  std::string output;
  do {
   if (!input_data || input_data_size <= 0)
    break;
   b64 = BIO_new(BIO_f_base64());
   if (!b64)
    break;
   if (!newline)
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
   bmem = BIO_new(BIO_s_mem());
   if (!bmem)
    break;
   b64 = BIO_push(b64, bmem);
   if (!b64)
    break;
   long bio_write_length = BIO_write(b64, input_data, static_cast<int>(input_data_size));
   if (bio_write_length <= 0)
    break;
   auto sslret = BIO_flush(b64);
   if (sslret != 1)
    break;
   sslret = BIO_get_mem_ptr(b64, &bptr);
   if (sslret != 1)
    break;
   sslret = BIO_set_close(b64, BIO_NOCLOSE);
   if (sslret != 1)
    break;
   output.append(bptr->data, bptr->length);
   result = true;
  } while (0);
  if (b64) {
   BIO_free_all(b64);
   b64 = nullptr;
  }
  if (bptr) {
   BUF_MEM_free(bptr);
   bptr = nullptr;
  }
  result_cb(output.data(), output.size());
  return result;
 }
 bool Ssl::DeBase64(const char* input_data, size_t input_data_size, const bool& newline, const tfResultDataCb& result_cb) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  std::string output;
  BIO* b64 = nullptr;
  BIO* bmem = nullptr;
  do {
   if (!input_data || input_data_size <= 0)
    break;
   b64 = BIO_new(BIO_f_base64());
   if (!b64)
    break;
   if (!newline)
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
   bmem = BIO_new_mem_buf(input_data, static_cast<int>(input_data_size));
   if (!bmem)
    break;
   bmem = BIO_push(b64, bmem);
   if (!bmem)
    break;
   output.resize(input_data_size);
   long sslret = BIO_read(bmem, &output[0], static_cast<int>(output.size()));
   if (sslret <= 0) {
    output.clear();
    break;
   }
   output.resize(sslret);
   result = true;
  } while (0);
  if (b64) {
   BIO_free_all(b64);
   b64 = nullptr;
  }
  result_cb(output.data(), output.size());
  return result;
 }

}///namespace local