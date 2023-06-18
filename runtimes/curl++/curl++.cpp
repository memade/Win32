#include "stdafx.h"

namespace local {

 Curl::Curl() {
  Init();
 }

 Curl::~Curl() {
  UnInit();
 }
 void Curl::Release() const {
  delete this;
 }
 void Curl::Init() {
  m_pConfig = new Config();
  curlpp::initialize(CURL_GLOBAL_ALL);
 }

 void Curl::UnInit() {
  curlpp::terminate();
  SK_DELETE_PTR(m_pConfig);
 }
 bool Curl::Start() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (m_IsOpen.load())
    break;


   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void Curl::Stop() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!m_IsOpen.load())
    break;

   m_IsOpen.store(false);

   for (auto it = m_Requests.begin(); it != m_Requests.end();) {
    it->second->Release();
    it = m_Requests.erase(it);
   }

   for (auto& it : m_Threads)
    it.join();
   m_Threads.clear();
  } while (0);
 }
 IConfig* Curl::ConfigGet() const {
  return m_pConfig;
 }
 bool Curl::DestroyRequest(const IRequest*& request) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!request)
    break;
   auto found = m_Requests.find(request->Identify());
   if (found == m_Requests.end())
    break;
   m_Requests.erase(found);
   result = true;
  } while (0);
  if (request) {
   dynamic_cast<const Request*>(request)->Release();
   request = nullptr;
  }
  return result;
 }
 IRequest* Curl::CreateRequest(const char* url) {
  IRequest* result = nullptr;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (!url)
   return result;
  auto newRequest = new Request(url, this);
  result = dynamic_cast<IRequest*>(m_Requests.emplace(newRequest->Identify(), newRequest).first->second);
  return result;
 }
 void Curl::OnRequestBegin(const IRequest* request) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_RequestBeginCb)
   m_RequestBeginCb(request);
 }
 void Curl::RegisterRequestBeginCb(const tfRequestDoneCb& end_cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_RequestBeginCb = end_cb;
 }
 void Curl::OnRequestDone(const IRequest* request) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_RequestDoneCb)
   m_RequestDoneCb(request);
 }
 void Curl::RegisterRequestDoneCb(const tfRequestDoneCb& end_cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_RequestDoneCb = end_cb;
 }
 void Curl::OnRequestRemove(const IRequest* request) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_RequestRemoveCb)
   m_RequestRemoveCb(request);
 }
 void Curl::RegisterRequestRemoveCb(const tfRequestRemoveCb& remove_cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_RequestRemoveCb = remove_cb;
 }
 void Curl::Process() {
  curlpp::Multi* pMultiObj = new curlpp::Multi();
  unsigned long long timer_perv = 0;
  std::vector<Request*> tasks;
  do {
   const unsigned long long timer_current = shared::Win::Time::TimeStamp<std::chrono::milliseconds>();

   do {
    if (timer_current - timer_perv < m_pConfig->StatusUpdateInterval())
     break;
    timer_perv = timer_current;
   } while (0);

   do {
    tasks.clear();
    std::unique_lock<std::mutex> lock(*m_Mutex, std::defer_lock);
    if (!lock.try_lock())
     break;
    for (auto it = m_Requests.begin(); it != m_Requests.end();) {
     if (it->second->Action() == RequestAction::REMOVE) {
      it->second->Release();
      it = m_Requests.erase(it);
      continue;
     }
     tasks.emplace_back(it->second);
     ++it;
    }
    lock.unlock();

    if (tasks.empty())
     break;

    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
     Request* pReqObj = *it;
     const auto action = pReqObj->Action();
     const auto status = pReqObj->Status();
     switch (action) {
     case RequestAction::PREPARTION: {
      if (status == RequestStatus::PREPARING)
       break;
      if (pReqObj->Finished()) {
       if (pReqObj->Success()) {
        OnRequestBegin(pReqObj);
        pReqObj->Action(RequestAction::START);
        pReqObj->UnFinished();
       }
       else {
        OnRequestDone(pReqObj);
        pReqObj->Action(RequestAction::STOP);
       }
       break;
      }
      pReqObj->Type(RequestType::HEAD);
      pMultiObj->add(pReqObj->Handle());
      pReqObj->Status(RequestStatus::PREPARING);
     }break;
     case RequestAction::START: {
      if (status == RequestStatus::STARTING)
       break;
      if (pReqObj->Finished()) {
       //pReqObj->FinalDone();
       OnRequestDone(pReqObj);
       pReqObj->Action(RequestAction::STOP);
       break;
      }
      pReqObj->TypeReal();
      pMultiObj->add(pReqObj->Handle());
      pReqObj->Status(RequestStatus::STARTING);
     }break;
     case RequestAction::PAUSE: {//!@ To be perfected!~
      pReqObj->Status(RequestStatus::PAUSED);
     }break;
     case RequestAction::STOP: {
      if (status == RequestStatus::STOPPING)
       break;
      pReqObj->Status(RequestStatus::STOPPING);
      pReqObj->Action(RequestAction::REMOVE);
     }break;
     default:
      break;
     }///switch
    }///for




    try {
     int nbLeft = 0;
     while (!pMultiObj->perform(&nbLeft)) {};
     if (nbLeft > 0) {
      int maxfd = 0;
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);
      struct timeval timeout = { 1,0 };
      pMultiObj->fdset(&fdread, &fdwrite, &fdexcep, &maxfd);
      int rc = ::select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
      if (rc < 0) {
       pMultiObj->clear(
        [](const curlpp::Easy* pEasySuccess, const curlpp::Easy* pEasyFailed) {


         auto sk = 0;
        });
       nbLeft = 0;
       break;
      }
      while (!pMultiObj->perform(&nbLeft)) {};
     }

     curlpp::Multi::Msgs msgs = pMultiObj->info();
     for (auto it = msgs.begin(); it != msgs.end();) {
      if (it->second.msg != CURLMSG::CURLMSG_DONE) {
       ++it;
       continue;
      }
      Request* reqObj = dynamic_cast<Request*>(const_cast<curlpp::Easy*>(it->first));
      reqObj->Status(RequestStatus::FINISHED);
      reqObj->Reason(it->second.code);
      pMultiObj->remove(it->first);
      it = msgs.erase(it);
     }///for

    }
    catch (curlpp::LogicError& e) {
     std::cout << e.what() << std::endl;
    }
    catch (curlpp::RuntimeError& e) {
     std::cout << e.what() << std::endl;
    }
    catch (...) {
     std::cout << "http common error." << std::endl;
    }

   } while (0);


   std::this_thread::sleep_for(std::chrono::milliseconds(m_pConfig->ProcessExecuteInterval()));
   if (!m_IsOpen.load()) {

    break;
   }
  } while (1);
  SK_DELETE_PTR(pMultiObj);
 }


 bool Curl::IsPreparationRequestType(const TypeRequestType& input_type) {
  return input_type & static_cast<TypeRequestType>(RequestType::HEAD);
 }
 RequestType Curl::GetRealRequestType(const TypeRequestType& input_type) {
  return static_cast<RequestType>(input_type & static_cast<TypeRequestType>(RequestType::BASE));
 }
}///namespace local 
