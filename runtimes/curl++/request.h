#if !defined(__8415F856_A191_4C6F_905D_47B5B1C9A511__)
#define __8415F856_A191_4C6F_905D_47B5B1C9A511__

namespace local {
 class Curl;
 class Request final : public IRequest, public curlpp::Easy {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Request(const std::string& url, const Curl*);
  virtual ~Request();
  void Release() const override final;
 public:
  const curlpp::Easy* Handle() const;
  TypeIdentify Identify() const override final;
 protected:
  void Header(const bool&) override final;
  const char* Url() const override final;
  void Type(const TypeRequestType&) override final;
  void Start() override final;
  void Stop() override final;
  void Pause() override final;
  void Remove() override final;
  void WriteCachePath(const char*) override final;
  bool EnableWriteCache(const bool&) override final;
  bool EnableWriteCache() const override final;
  void RegisterProgressCb(const tfProgressCb&) override final;
  void RegisterWriteStreamCb(const tfWriteStreamCb&) override final;
  void ConnectTimeout(const std::uint32_t&) override final;
  void Timeout(const std::uint32_t&) override final;
  size_t ContentLength() const override final;
  void Body(const tfCBufferRouteCb&) const override final;
  const ResultCode& Reason() const override final;
 public:
  bool Success() const override final;
  void Type(const RequestType&);
  void Action(const RequestAction&);
  bool Preparation() const;
  void TypeReal();
  RequestType Type() const override final;
  void Status(const RequestStatus&);
  void Reason(const CURLcode&);
  bool Finished() const;
  void FinalDone();
  void UnFinished();
  const RequestStatus& Status() const override final;
  const RequestAction& Action() const;
  const char* UrlHost() const override final;
  const char* UrlPath() const override final;
  const char* UrlSchema() const override final;
  const char* UrlFileName() const override final;
  const char* UrlFileFormat() const override final;
  const char* WriteCachePath() const override final;
  const char* CacheFileNameFinal() const override final;
  const char* CacheFileNameTemp() const override final;
  void Verbose(const bool&) override final;
  std::string GetWriteStreamBuffer() const;
 private:
  bool WriteStream(const char*, const size_t&);
 private:
  const std::string m_Url;
  std::string m_UrlHost;
  std::string m_UrlPath;
  std::string m_UrlSchema;
  std::string m_UrlFileName;
  std::string m_UrlFileFormat;
  std::string m_WriteCachePath;
  std::string m_WriteCachePathname;
  std::string m_CacheFileNameFinal;
  std::string m_CacheFileNameTemp;
  const Curl* m_pHost;
  const TypeIdentify m_Identify;
  TypeRequestType m_Type = static_cast<TypeRequestType>(RequestType::UNKNOWN);
  RequestStatus m_Status = RequestStatus::UNKNOWN;
  RequestAction m_Action = RequestAction::UNKNOWN;
  ResultCode m_Reason = ResultCode::UNKNOWN;
  tfProgressCb m_ProgressCb = nullptr;
  tfWriteStreamCb m_WriteStreamCb = nullptr;
  Cache* m_pFileCache = nullptr;
  std::ostringstream* m_pOstringstream = nullptr;
  Headers* m_pHeadersRequest = nullptr;
  Headers* m_pHeadersResponse = nullptr;
  std::atomic_bool m_EnableWriteCache = false;
 };
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 05 Jun 2023 14:09:14 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__8415F856_A191_4C6F_905D_47B5B1C9A511__
