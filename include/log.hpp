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

      if (Empty())
       std::this_thread::sleep_for(std::chrono::milliseconds(10));
      if (!m_IsOpen.load())
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
   std::unique_lock<std::mutex> lock{ *m_Mutex, std::defer_lock };
   lock.lock();
   m_OutputQ.push(log_data);
   lock.unlock();
  }
  bool Empty() const {
   bool result = false;
   std::unique_lock<std::mutex> lock{ *m_Mutex, std::defer_lock };
   lock.lock();
   result = m_OutputQ.empty();
   lock.unlock();
   return result;
  }
 private:
  void Output() {
   std::unique_lock<std::mutex> lock{ *m_Mutex, std::defer_lock };
   lock.lock();
   do {
    if (m_OutputQ.empty())
     break;
    std::string data = m_OutputQ.front();
    SYSTEMTIME stime = { 0 };
    ::GetLocalTime(&stime);
    data.insert(0, std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}/{:03}\t",
     stime.wYear, stime.wMonth, stime.wDay,
     stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds));
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

template <class... _Types>
static void LOG_OUTPUT(const _STD format_string<_Types...> _Fmt, _Types&&... _Args) {
 do {
  if (!__gspLog)
   break;
  _STD string s = _STD vformat(_Fmt.get(), _STD make_format_args(_Args...));
  if (s.empty())
   break;
  *__gspLog << s;
 } while (0);
}

template <class... _Types>
static void LOG_MSGBOX(const _STD format_string<_Types...> _Fmt, _Types&&... _Args) {
 _STD string s = _STD vformat(_Fmt.get(), _STD make_format_args(_Args...));
 ::MessageBoxA(NULL, s.c_str(), __FUNCTION__, MB_TOPMOST);
}
template <class... _Types>
static void LOG_MSGBOX(const _STD wformat_string<_Types...> _Fmt, _Types&&... _Args) {
 _STD wstring s = _STD vformat(_Fmt.get(), _STD make_wformat_args(_Args...));
 ::MessageBoxW(NULL, s.c_str(), __FUNCTIONW__, MB_TOPMOST);
}

#if 0//!@ Not supported at present
template <class... _Types>
static void LOG_OUTPUT(const _STD wformat_string<_Types...> _Fmt, _Types&&... _Args) {
 do {
  _STD wstring s = _STD vformat(_Fmt.get(), _STD make_wformat_args(_Args...));
  if (s.empty())
   break;
  *__gspLog << s;
 } while (0);
}
#endif


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

#if 0


#define LOG_OUTPUT(s) \
do{\
if (!__gspLog)\
break;\
if (s.empty())\
break;\
*__gspLog << s;\
} while (0);\

#endif





/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 27 Apr 2023 08:29:20 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B4907D76_FA9F_49AD_AC90_62EB586B8F99__