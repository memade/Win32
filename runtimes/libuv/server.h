#if !defined(__16AB8F75_13DF_4230_9176_E86A1B4E8818__)
#define __16AB8F75_13DF_4230_9176_E86A1B4E8818__

namespace local {

 class Server final : public ILibuv , public IServer {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Server();
  virtual ~Server();
 private:
  void Init();
  void UnInit();
 public:
  bool Start() override final;
  void Stop() override final;
  ServerStatus Status() const override final;
  void Release() const override final;
  IConfig* ConfigGet() const override final;
 private:
  void OnConnent(Session*);
  void Process();
 private:
  shared::container::map<u_short, Session*> m_Sessions;
  std::vector<std::thread> m_Threads;
  std::atomic_bool m_IsOpen = false;
  Config* m_pConfig = nullptr;
  std::atomic<ServerStatus> m_Status = ServerStatus::Closed;
  shared::Log* m_pLog = nullptr;
 private:
  static void AsyncCb(uv_async_t* async);
  static void ConnectionCb(uv_stream_t* server, int status);
  static void AllocCb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);
  static void UdpRecvCb(uv_udp_t* handle,ssize_t nread,const uv_buf_t* buf,const struct sockaddr* addr,unsigned flags);
 };


 extern Server* __gpServer;
}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:22:30 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__16AB8F75_13DF_4230_9176_E86A1B4E8818__

