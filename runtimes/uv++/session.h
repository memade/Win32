#if !defined(__36EC4FD1_7F29_4A18_B369_08BF719C8B8A__)
#define __36EC4FD1_7F29_4A18_B369_08BF719C8B8A__

namespace local {

 struct write_req_t {
  uv_write_t write;
  uv_handle_t* handle;
  uv_buf_t buf;

  write_req_t() {
   memset(this, 0x00, sizeof(*this));
   write.data = this;
  }
  ~write_req_t() { SK_DELETE_PTR_BUFFER(buf.base); }
 };

 typedef class Session final : public ISession {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Session();
  ~Session();
 public:
  std::string Read();
  write_req_t* Write();
  bool Read(const char*, const size_t&);
  bool Write(const CommandType&, const std::string&);
  void Handle(uv_handle_t*);
  uv_handle_t* Handle() const;
  void Caller(void*);
  void* Caller() const;
  void Route(void*);
  void* Route() const;
  void Status(const SessionStatus&);
  const SessionStatus& Status() const;
  void Address(const std::string&);
  const std::string& Address() const;
  void SockAddr(const sockaddr&);
  void SockAddr(const sockaddr_in&);
  const sockaddr& SockAddr() const;
  void SetReadBuf(uv_buf_t*&) const;
  unsigned long long ActivationTime(const unsigned long long&);
  void ServerTypeSet(const ServerType&);
  const ServerType& ServerTypeGet() const;
  void SessionTypeSet(const SessionType&);
  const SessionType& SessionTypeGet() const;
 private:
  std::string m_Address;
  uv_handle_t* handle_ = nullptr;
  void* m_pCaller = nullptr;
  void* m_pRoute = nullptr;
  SessionStatus m_Status = SessionStatus::UNKNOWN;
  ServerType m_ServerType = ServerType::UNKNOWN;
  SessionType m_SessionType = SessionType::UNKNOWN;
  struct sockaddr m_SockAddr = { 0 };
  Stream* m_pReadStream = nullptr;
  Stream* m_pWriteStream = nullptr;
  std::atomic_ullong m_ActivationTime = 0;
 }UserData;

}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:27:20 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__36EC4FD1_7F29_4A18_B369_08BF719C8B8A__