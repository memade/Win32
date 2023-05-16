#if !defined(__B4907D76_FA9F_49AD_AC90_62EB586B8F99__)
#define __B4907D76_FA9F_49AD_AC90_62EB586B8F99__

namespace shared {

 class Log final {
 public:
  Log() {
   m_Mutex = std::make_shared<std::mutex>();
   m_IsOpen.store(true);
   m_ProcessThread = std::thread(
    [this]() {
     do {    
      Output();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      if (!m_IsOpen.load() && Empty())
       break;
     } while (1);
    });
  }
  ~Log() {
   m_IsOpen.store(false);
   if (m_ProcessThread.joinable())
    m_ProcessThread.join();
  }
 public:
  void operator<<(const std::string& log_data) {
   std::unique_lock<std::mutex> lock{ *m_Mutex ,std::defer_lock };
   lock.lock();
   m_OutputQ.push(log_data);
   lock.unlock();
  }
  bool Empty() const {
   bool result = false;
   std::unique_lock<std::mutex> lock{ *m_Mutex ,std::defer_lock };
   lock.lock();
   result = m_OutputQ.empty();
   lock.unlock();
   return result;
  }
 private:
  void Output() {
   std::unique_lock<std::mutex> lock{ *m_Mutex ,std::defer_lock };
   lock.lock();
   do {
    if (m_OutputQ.empty())
     break;
    std::string data = m_OutputQ.front();
    SYSTEMTIME stime = { 0 };
    ::GetLocalTime(&stime);
    data.insert(0,std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}/{:03}\t",
     stime.wYear,stime.wMonth,stime.wDay,
     stime.wHour, stime.wMinute,stime.wSecond,stime.wMilliseconds));
    std::cout << data << std::endl;
    m_OutputQ.pop();
   } while (0);
   lock.unlock();
  }
 private:
  std::shared_ptr<std::mutex> m_Mutex;
  std::atomic_bool m_IsOpen = false;
  std::thread m_ProcessThread;
  std::queue<std::string> m_OutputQ;
 };



}///namespace shared


static shared::Log* __gspLog = nullptr;

#define LOG_INIT \
do{\
if (!__gspLog){\
__gspLog = new shared::Log();\
*__gspLog << "Init log module.";\
}\
} while (0);

#define LOG_UNINIT \
do{\
if(__gspLog){\
*__gspLog << "Uninit log module.";\
delete __gspLog;\
__gspLog = nullptr;\
}\
} while (0);\

#define LOG_OUTPUT(s) \
do{\
if (!__gspLog)\
break;\
if (s.empty())\
break;\
*__gspLog << s;\
} while (0);\

/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 27 Apr 2023 08:29:20 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B4907D76_FA9F_49AD_AC90_62EB586B8F99__