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
      std::unique_lock<std::mutex> lock{ *m_Mutex ,std::defer_lock };

      lock.lock();
      do {
       if (m_OutputQ.empty())
        break;
       std::string data = m_OutputQ.back();
       std::cout << data << std::endl;
       m_OutputQ.pop();
      } while (0);
      lock.unlock();

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
   std::unique_lock<std::mutex> lock{ *m_Mutex ,std::defer_lock };
   lock.lock();
   m_OutputQ.push(log_data);
   lock.unlock();
  }
 private:
  std::shared_ptr<std::mutex> m_Mutex;
  std::atomic_bool m_IsOpen = false;
  std::thread m_ProcessThread;
  std::queue<std::string> m_OutputQ;
 };



}///namespace shared

#define LOG_OUTPUT(log_ptr,s) \
do{\
if (!log_ptr)\
break;\
if (s.empty())\
break;\
*log_ptr << s;\
} while (0);\

/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 27 Apr 2023 08:29:20 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__B4907D76_FA9F_49AD_AC90_62EB586B8F99__