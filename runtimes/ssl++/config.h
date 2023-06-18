#if !defined(__DF04609E_A0A4_4236_8390_A763A9F4678B__)
#define __DF04609E_A0A4_4236_8390_A763A9F4678B__

namespace local {

 class Config final : public IConfig {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Config();
  ~Config();
 private:
  void Init();
  void UnInit();

 };
}///namespace lcoal

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 20 May 2023 01:20:31 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DF04609E_A0A4_4236_8390_A763A9F4678B__