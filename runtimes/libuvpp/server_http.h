#if !defined(__0EBC3392_E2CE_42F0_A22F_E9AED2A6517F__)
#define __0EBC3392_E2CE_42F0_A22F_E9AED2A6517F__

namespace local {

 class ServerHttp final : public IServerHttp {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  ServerHttp();
  virtual ~ServerHttp();
 private:
  void Init();
  void UnInit();
 public:
   bool Start() override final;
   void Stop() override final;
  void Release() const override final;
 private:
  uv::EventLoop m_Loop;
  uv::http::HttpServer* m_pHttpServer = nullptr;
 };

}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 26 Jan 2023 03:22:14 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0EBC3392_E2CE_42F0_A22F_E9AED2A6517F__

