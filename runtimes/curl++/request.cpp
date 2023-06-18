#include "stdafx.h"

namespace local {
 Request::Request(const std::string& url, const Curl* pHost)
  : m_pHost(pHost)
  , m_Url(shared::Win::UrlFixedA(url))
  , m_Identify(shared::Win::Time::TimeStamp<std::chrono::microseconds>()) {
  m_pOstringstream = new std::ostringstream;
  m_pHeadersRequest = new Headers();
  m_pHeadersResponse = new Headers();

  m_WriteCachePath = \
   reinterpret_cast<Config*>(m_pHost->ConfigGet())->DownCacheDirectory();
  shared::Win::GetFileNameAndFormat(m_Url, m_UrlFileName, m_UrlFileFormat);
  httpparser::HttpUrlFields urlFields;
  httpparser::Method::parse_http_url(url, urlFields);
  m_UrlHost = urlFields[httpparser::EnHttpUrlFieldType::UF_HOST];
  m_UrlPath = urlFields[httpparser::EnHttpUrlFieldType::UF_PATH];
  m_UrlSchema = urlFields[httpparser::EnHttpUrlFieldType::UF_SCHEMA];
  m_CacheFileNameFinal = m_UrlFileName + m_UrlFileFormat;
  shared::Win::Encryption::MD5(m_Url, m_CacheFileNameTemp);
  m_CacheFileNameTemp = shared::Win::BinToHex(m_CacheFileNameTemp);
  setOpt(new curlpp::options::Url(m_Url));


  if (m_pHost->ConfigGet()->EnableSSLSupport()) {
   //!@ 支持https | openssl
   setOpt(new curlpp::options::SslVerifyHost(0));
   setOpt(new curlpp::options::SslVerifyPeer(0));
  }

  /// 以秒为单位：
  /// curl_setopt($ch, CURLOPT_TIMEOUT, 1);
  /// 以毫秒为单位：在CURL7.16.2中被加入，从PHP5.2.3起可使用
  /// curl_setopt($ch, CURLOPT_NOSIGNAL, true);
  /// curl_setopt($ch, CURLOPT_TIMEOUT_MS, 200);
  /// 以毫秒为单位：在CURL7.16.2中被加入，从PHP5.2.3起可使用
  setOpt(new curlpp::options::NoSignal(true));
  /// 如果设置的URL不存在的话，服务器返回404错误，但是程序发现不了错误，还是会下载这个404页面。
  /// 这时需要设置CURLOPT_FAILONERROR属性，当HTTP返回值大于等于400的时候
  setOpt(new curlpp::options::FailOnError(true));
  /// 使用默认发起连接超时
  setOpt(new curlpp::options::ConnectTimeout(0));
  /// 使用默认超时
  setOpt(new curlpp::options::Timeout(0));
  /// 不启用进度回调
  setOpt(new curlpp::options::NoProgress(false));
  /// 接收缓冲
  setOpt(new curlpp::options::WriteStream(m_pOstringstream));
  /// 默认请求头回调
  setOpt(new curlpp::options::HeaderFunction(
   [this](char* buffer, size_t size, size_t items)->size_t {
    size_t result = size * items;
    do {
     if (!buffer || result <= 0) {
      result = 0;
      break;
     }
     *m_pHeadersResponse << std::string(buffer, result);
    } while (0);
    return result;
   }));
  //!@ 默认写数据回调
  m_WriteStreamCb = [this](char* buffer, size_t size, size_t items)->size_t {
   size_t result_size = size * items;
   do {
    if (!buffer || result_size <= 0) {
     result_size = 0;
     break;
    }
    if (Action() == RequestAction::STOP) {
     result_size = 0;
     break;
    }

    if (Action() == RequestAction::PREPARTION ||
     Status() == RequestStatus::PREPARING ||
     Status() == RequestStatus::PREPARAED)
     break;

    if (!WriteStream(buffer, result_size)) {
     result_size = 0;
     break;
    }
   } while (0);
   return result_size;
  };
 }
 Request::~Request() {
  SK_DELETE_PTR(m_pFileCache);
  SK_DELETE_PTR(m_pHeadersRequest);
  SK_DELETE_PTR(m_pHeadersResponse);
  SK_DELETE_PTR(m_pOstringstream);
 }
 void Request::Release() const {
  delete this;
 }
 void Request::Body(const tfCBufferRouteCb& alloc_cb) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!alloc_cb)
    break;
   std::string body;
   if (m_pFileCache) {
    *m_pFileCache >> body;
   }
   else if (m_pOstringstream) {
    body = m_pOstringstream->str();
   }
   else
    break;
   if (body.empty())
    break;
   const char* buffer = body.data();
   size_t buffer_size = body.size();
   alloc_cb(buffer, buffer_size);
  } while (0);
 }
 size_t Request::ContentLength() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  std::string found = m_pHeadersResponse->search(R"(Content-Length)");
  return strtoull(found.c_str(), nullptr, 10);
 }
 void Request::ConnectTimeout(const std::uint32_t& time_s) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  setOpt(new curlpp::options::ConnectTimeout(time_s));
 }
 void Request::Timeout(const std::uint32_t& time_s) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  setOpt(new curlpp::options::Timeout(time_s));
 }
 TypeIdentify Request::Identify() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Identify;
 }
 const RequestAction& Request::Action() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Action;
 }
 void Request::Status(const RequestStatus& status) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Status = status;
 }
 const RequestStatus& Request::Status() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Status;
 }
 void Request::Action(const RequestAction& action) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Action = action;
 }
 void Request::Start() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  Curl::IsPreparationRequestType(m_Type) ? \
   m_Action = RequestAction::PREPARTION : m_Action = RequestAction::START;
 }
 void Request::Stop() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Action = RequestAction::STOP;
 }
 void Request::Pause() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Action = RequestAction::PAUSE;
 }
 void Request::Remove() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Action = RequestAction::REMOVE;
 }
 const curlpp::Easy* Request::Handle() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return dynamic_cast<const curlpp::Easy*>(this);
 }
 const char* Request::Url() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Url.c_str();
 }
 void Request::Header(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  setOpt(new curlpp::options::Header(enable));
 }
 const ResultCode& Request::Reason() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Reason;
 }
 void Request::UnFinished() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Reason = ResultCode::UNKNOWN;
 }
 void Request::Verbose(const bool& enable) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  setOpt(new curlpp::options::Verbose(enable));
 }
 void Request::FinalDone() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!shared::Win::AccessA(m_WriteCachePathname))
    break;
   auto final_filepathname = \
    shared::Win::PathFixedA(shared::Win::GetPathByPathnameA(m_WriteCachePathname) + "\\" + m_CacheFileNameFinal);
   ::remove(final_filepathname.c_str());
   ::rename(m_WriteCachePathname.c_str(), final_filepathname.c_str());
  } while (0);
 }
 bool Request::Finished() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return static_cast<TypeResultCodeType>(m_Reason) >= 0;
 }
 void Request::Reason(const CURLcode& reason) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_Reason = static_cast<decltype(m_Reason)>(reason);
 }
 RequestType Request::Type() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return Curl::GetRealRequestType(m_Type);
 }
 void Request::Type(const TypeRequestType& type) {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  m_Type = type;
  lock.unlock();
 }
 void Request::TypeReal() {
  Type(Curl::GetRealRequestType(m_Type));
 }
 void Request::Type(const RequestType& type) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pOstringstream->clear();
  switch (type) {
  case RequestType::HEAD: {
   setOpt(new curlpp::options::Header(true));
   setOpt(new curlpp::options::NoBody(true));
   setOpt(new curlpp::options::HttpGet(false));
   setOpt(new curlpp::options::Post(false));
   setOpt(new curlpp::options::WriteStream(m_pOstringstream));
   setOpt(new curlpp::options::ResumeFromLarge(0));
  }break;
  case RequestType::DOWN: {
   setOpt(new curlpp::options::Header(false));
   setOpt(new curlpp::options::NoBody(false));
   setOpt(new curlpp::options::HttpGet(true));
   setOpt(new curlpp::options::Post(false));
   if (m_EnableWriteCache.load()) {
    setOpt(new curlpp::options::WriteStream(nullptr));
    setOpt(new curlpp::options::ResumeFromLarge(m_pFileCache ? static_cast<curl_off_t>(static_cast<long long>(m_pFileCache->Size())) : 0));
    setOpt(new curlpp::options::WriteFunction(m_WriteStreamCb));
   }
   else
    setOpt(new curlpp::options::WriteStream(m_pOstringstream));
  }break;
  case RequestType::UPLOAD: {
#if 0
   setOpt(new curlpp::options::Header(false));
   setOpt(new curlpp::options::WriteStream(nullptr));
   setOpt(new curlpp::options::WriteFunction(m_WriteStreamCb));
#endif
  }break;
  case RequestType::GET: {
   setOpt(new curlpp::options::Header(false));
   setOpt(new curlpp::options::NoBody(false));
   setOpt(new curlpp::options::HttpGet(true));
   setOpt(new curlpp::options::Post(false));
   if (m_EnableWriteCache.load()) {
    setOpt(new curlpp::options::WriteStream(nullptr));
    setOpt(new curlpp::options::ResumeFromLarge(m_pFileCache ? static_cast<curl_off_t>(static_cast<long long>(m_pFileCache->Size())) : 0));
    setOpt(new curlpp::options::WriteFunction(m_WriteStreamCb));
   }
   else
    setOpt(new curlpp::options::WriteStream(m_pOstringstream));
  }break;
  case RequestType::POST: {
   setOpt(new curlpp::options::Header(false));
   setOpt(new curlpp::options::NoBody(false));
   setOpt(new curlpp::options::HttpGet(false));
   setOpt(new curlpp::options::Post(true));
   if (m_EnableWriteCache.load()) {
    setOpt(new curlpp::options::WriteStream(nullptr));
    setOpt(new curlpp::options::ResumeFromLarge(m_pFileCache ? static_cast<curl_off_t>(static_cast<long long>(m_pFileCache->Size())) : 0));
    setOpt(new curlpp::options::WriteFunction(m_WriteStreamCb));
   }
   else
    setOpt(new curlpp::options::WriteStream(m_pOstringstream));
  }break;
  default:
   break;
  }
 }
 std::string Request::GetWriteStreamBuffer() const {
  std::string result;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  result = m_pOstringstream->str();
  return result;
 }
 bool Request::WriteStream(const char* buffer, const size_t& buffer_size) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (buffer && buffer_size > 0 && m_pFileCache)
   result = *m_pFileCache << std::string(buffer, buffer_size);
  return result;
 }
 const char* Request::UrlHost() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_UrlHost.c_str();
 }
 const char* Request::UrlPath() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_UrlPath.c_str();
 }
 const char* Request::UrlSchema() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_UrlSchema.c_str();
 }
 const char* Request::UrlFileName() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_UrlFileName.c_str();
 }
 const char* Request::UrlFileFormat() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_UrlFileFormat.c_str();
 }
 const char* Request::WriteCachePath() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_WriteCachePath.c_str();
 }
 const char* Request::CacheFileNameFinal() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_CacheFileNameFinal.c_str();
 }
 const char* Request::CacheFileNameTemp() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_CacheFileNameTemp.c_str();
 }
 bool Request::Preparation() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return Curl::IsPreparationRequestType(m_Type);
 }
 bool Request::Success() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_Reason == ResultCode::OK;
 }
 void Request::WriteCachePath(const char* path) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (!path)
   return;
  m_WriteCachePath = path;
 }
 bool Request::EnableWriteCache(const bool& enable) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   m_EnableWriteCache.store(enable);
   if (!m_EnableWriteCache.load()) {
    SK_DELETE_PTR(m_pFileCache);
    result = true;
    if (!m_WriteCachePath.empty()) {
     auto cache_filepathname = shared::Win::PathFixedA(m_WriteCachePath + "\\" + m_CacheFileNameTemp);
     if (shared::Win::AccessA(cache_filepathname))
      ::remove(cache_filepathname.c_str());
    }
    break;
   }

   if (!shared::Win::AccessA(m_WriteCachePath))
    shared::Win::CreateDirectoryA(m_WriteCachePath);
   if (!shared::Win::AccessA(m_WriteCachePath))
    break;
   m_WriteCachePathname = shared::Win::PathFixedA(m_WriteCachePath + "\\" + m_CacheFileNameTemp);
   SK_DELETE_PTR(m_pFileCache);
   m_pFileCache = new Cache(m_WriteCachePathname);
   if (!m_pFileCache->Ready()) {
    SK_DELETE_PTR(m_pFileCache);
    break;
   }
   result = true;
  } while (0);
  return result;
 }
 bool Request::EnableWriteCache() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_EnableWriteCache.load();
 }
 void Request::RegisterWriteStreamCb(const tfWriteStreamCb& cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_WriteStreamCb = cb;
#if 0
  //!@ http headers are not buffered
  do {
   if (!m_WriteStreamFile.empty()) {
    SK_DELETE_PTR(m_pFileCache);
    m_pFileCache = new Cache(m_WriteStreamFile);
    if (!m_pFileCache->Ready()) {
     SK_DELETE_PTR(m_pFileCache);
     break;
    }

    setOpt(new curlpp::options::ResumeFromLarge(static_cast<curl_off_t>(static_cast<long long>(m_pFileCache->Size()))));
   }

   setOpt(new curlpp::options::WriteFunction(\
    m_WriteStreamCb ? m_WriteStreamCb : \
    [this](char* buffer, size_t size, size_t items)->size_t {
     size_t result_size = size * items;
     do {
      if (!buffer || result_size <= 0) {
       result_size = 0;
       break;
      }
      if (Action() == RequestAction::STOP) {
       result_size = 0;
       break;
      }

      if (Action() == RequestAction::PREPARTION ||
       Status() == RequestStatus::PREPARING ||
       Status() == RequestStatus::PREPARAED)
       break;

      if (!WriteStream(buffer, result_size)) {
       result_size = 0;
       break;
      }
     } while (0);
     return result_size;
}));
  } while (0);
#endif
 }
 void Request::RegisterProgressCb(const tfProgressCb& cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_ProgressCb = cb;
  if (!cb) {
   setOpt(new curlpp::options::NoProgress(true));
   setOpt(new curlpp::options::ProgressFunction(nullptr));
  }
  else {
   setOpt(new curlpp::options::NoProgress(false));
   setOpt(new curlpp::options::ProgressFunction(
    [this](double dltotal, double dlnow, double ultotal, double ulnow)->int {
     return 0;
    }));
  }
 }
}///namespace local
