#if !defined(INC_H___B1C7EF7F_703D_4A25_BFA4_3E58F4D0096F__HEAD__)
#define INC_H___B1C7EF7F_703D_4A25_BFA4_3E58F4D0096F__HEAD__

namespace local {
 class Session final : public ISession {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Session(const TypeIdentify&, const std::string&, uv::TcpConnection*);
  virtual ~Session();
 public:
  const TypeIdentify& Identify() const override final;
  const std::string& Name() const override final;
  void Write(const unsigned long long&, const std::string&) override final;
  void Read(std::vector<std::string>&) override final;
  bool Ready() const override final;
  void Read(std::string&) override final;
  void Read(const tfSessionReadCb&) override final;
  void Write(const std::string&) override final;
  void BindUL(const unsigned long&) override final;
  const unsigned long& BindUL() const override final;
  void BindULL(const unsigned long long&) override final;
  const unsigned long long& BindULL() const override final;
  void BindStr(const std::string&) override final;
  const std::string& BindStr() const override final;
  void BindPtr(void*) override final;
  void* BindPtr() const override final;
  void BindProcessId(const unsigned long&) override final;
  const unsigned long& BindProcessId() const override final;
  void BindTaskId(const unsigned long long&) override final;
  const unsigned long long& BindTaskId() const override final;
 public:
  void Write();
  void Release() const override final;
  void Read(const char*, const size_t&);
 private:
  const TypeIdentify m_Identify;
  const std::string m_Name;
  shared::container::buffer m_BufferRead;
  shared::container::buffer m_BufferWrite;
  uv::TcpConnection* m_pUvConnection = nullptr;
  void* m_BindPtr = nullptr;
  std::string m_BindStr;
  unsigned long long m_BindULL = 0;
  unsigned long m_BindUL = 0;
  unsigned long m_ProcessId = 0;
  unsigned long long m_TaskId = 0;
 };

}///namespace local

/// /*新生®（上海）**/
/// /*2022_10_14T22:05:07.5878580Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___B1C7EF7F_703D_4A25_BFA4_3E58F4D0096F__HEAD__