#if !defined(__81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__)
#define __81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__

#include <openssl/err.h>
//#include <openssl/types.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/hmac.h>
#include <openssl/md2.h>
#include <openssl/md5.h>
#include <openssl/rc5.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/buffer.h>

namespace local {

 class Ssl final : public ISsl {
 public:
  Ssl();
  virtual ~Ssl();
 protected:
  void Release() const override final;
  bool EnMD5(const char*, size_t, const tfResultDataCb&) const override final;
  bool EnBase64(const char*, size_t, const bool& newline, const tfResultDataCb&) const override final;
  bool DeBase64(const char*, size_t, const bool& newline, const tfResultDataCb&) const override final;
 public:
  IConfig* ConfigGet() const override final;
 private:
  void Init();
  void UnInit();
  Config* m_pConfig = nullptr;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:26:50 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__81FDDD44_91F0_40B1_B7F8_9749F23B1A2E__
