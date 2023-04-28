#if !defined(__A3A521A0_3093_4E00_A03D_9CEB29CFC6B5__)
#define __A3A521A0_3093_4E00_A03D_9CEB29CFC6B5__

namespace local {

 class Stream final {
 private:
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Stream(const size_t& buffer_size, const StreamType&);
  ~Stream();
 public:
  bool operator<<(const std::string&);
  bool operator>>(std::string&);
  void Resize(const size_t& buffer_size);
  void Reset();
  char* Base() const;
  const size_t& BufferSize() const;
  bool Empty() const;
 private:
  size_t m_BufferSize;
  char* m_LevelCache1 = nullptr;
  std::string m_LevelCache2;
  const StreamType m_StreamType;
  std::queue<std::string> m_LevelCache3;
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:20:41 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__A3A521A0_3093_4E00_A03D_9CEB29CFC6B5__