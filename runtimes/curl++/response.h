#if !defined(__A591D5DF_0B0F_48AD_A666_CCC55EB3E81F__)
#define __A591D5DF_0B0F_48AD_A666_CCC55EB3E81F__

namespace local {
 class Response final : public IResponse {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Response();
  virtual ~Response();
 };
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 05 Jun 2023 14:10:48 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__A591D5DF_0B0F_48AD_A666_CCC55EB3E81F__
