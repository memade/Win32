#if !defined(__675D16B5_C232_46BE_8996_36A05DB12580__)
#define __675D16B5_C232_46BE_8996_36A05DB12580__

namespace local {

 class Server final : public IServer {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Server();
  ~Server();
 public:
  bool Start() override final;
  void Stop() override final;
  ServerStatus Status() const override final;
  unsigned long SessionCount() const override final;
  Config* ConfigGet() const override final;
  void Release() const override final;
 public:
  bool IsClose() const;
  void Status(const ServerStatus&);
  void SessionCount(const unsigned long&);
 private:
  void Init();
  void UnInit();
  static void MainProcess(void*);
  static void WorkProcess(uv_handle_t* handle, void* arg);
  static void SessionConnectionCb(uv_stream_t* server, int status);
  HANDLE thread_main_ = nullptr;
  std::atomic_bool m_IsOpen = false;
  std::atomic_ulong m_SessionCount = 0;
  Config* m_pConfig = nullptr;
  ServerStatus m_ServerStatus = ServerStatus::UNKNOWN;
 };


 extern Server* __gpServer;
}///namespace local 


/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:27:11 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__675D16B5_C232_46BE_8996_36A05DB12580__