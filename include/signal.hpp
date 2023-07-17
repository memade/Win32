#if !defined(__843EC316_8803_45FA_9D47_714D645195CF__)
#define __843EC316_8803_45FA_9D47_714D645195CF__

namespace shared {

 using tfOnSignalTriggerCb = std::function<void(const class SignalNode*)>;

 enum class SignalNodeStatus : unsigned long {
  unknown = 0,
  activate = 1,
  trigger = activate << 1,
  timeout = activate << 2,
  finish = trigger | timeout,
  shutdown = finish << 1,
 };
 typedef class SignalNode final {
 public:
  SignalNode(const std::uint64_t& identify)
   : identify_(identify) {
  }
  ~SignalNode() {
  }
  void Release() const {
   delete this;
  }
 public:
  const std::uint64_t& identify() const {
   std::lock_guard<std::mutex>(*m_Mutex);
   return identify_;
  }
#if 0
  bool wait(const std::uint64_t& timeout_ms = 0) {
   bool result = false;
   status(SignalNodeStatus::activate);
   std::atomic_uint64_t wait_total_time = \
    timeout_ms <= 0 ? (0xFFFFFFFF - (0xFFFFFFFF % 100)) : timeout_ms;
   wait_total_time += (100 - (wait_total_time % 100));
   do {
    if (status() == SignalNodeStatus::shutdown)
     break;
    std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
    lock.lock();
    if (wait_total_time <= 0)
     break;
    wait_total_time -= 100;
    if (trigger_.load()) {
     result = true;
     break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    lock.unlock();
   } while (1);
   status(result ? SignalNodeStatus::success : SignalNodeStatus::failed);
   return result;
  }
#endif
  void trigger() {
   std::lock_guard<std::mutex>(*m_Mutex);
   status_.store(SignalNodeStatus::trigger);
  }
  bool finish() const {
   std::lock_guard<std::mutex>(*m_Mutex);
   /*unsigned long current = static_cast<unsigned long>(status_.load());*/
   /*return (current & static_cast<unsigned long>(SignalNodeStatus::failed)) || \
    (current & static_cast<unsigned long>(SignalNodeStatus::success));*/
   return status_.load() == SignalNodeStatus::trigger ||
    status_.load() == SignalNodeStatus::timeout ||
    status_.load() == SignalNodeStatus::shutdown;
  }
  SignalNodeStatus status() const {
   std::lock_guard<std::mutex>(*m_Mutex);
   return status_.load();
  }
  void status(const SignalNodeStatus& s) {
   std::lock_guard<std::mutex>(*m_Mutex);
   status_.store(s);
  }
  void timeout(const std::uint64_t& t) {
   std::lock_guard<std::mutex>(*m_Mutex);
   timeout_ = t <= 0 ? (0xFFFFFFFF - (0xFFFFFFFF % 100)) : t;
   timeout_ += (100 - (timeout_ % 100));
  }
  const std::uint64_t& timeout() const {
   std::lock_guard<std::mutex>(*m_Mutex);
   return timeout_;
  }
  void RegisterOnSignalTriggerCb(const tfOnSignalTriggerCb& cb) {
   std::lock_guard<std::mutex>(*m_Mutex);
   on_signal_trigger_cb_ = cb;
  }
  void OnSignalTrigger() const {
   std::lock_guard<std::mutex>(*m_Mutex);
   if(on_signal_trigger_cb_)
    on_signal_trigger_cb_(this);
  }
 private:
  const std::uint64_t identify_;
  std::uint64_t timeout_ = 0;
  tfOnSignalTriggerCb on_signal_trigger_cb_ = nullptr;
  std::atomic<SignalNodeStatus> status_ = SignalNodeStatus::unknown;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 }SignalHandle;

 class Signal final {
 public:
  inline Signal();
  inline ~Signal();
  inline void Release() const;
 public:
  inline SignalNode* create();
  inline SignalNode* create(const std::uint64_t& timeout, const tfOnSignalTriggerCb&);
  inline SignalNode* search(const std::uint64_t&) const;
  inline void wait();
 private:
  std::map<std::uint64_t, SignalNode*> nodes_;
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 };
 inline Signal::Signal() {

 }

 inline Signal::~Signal() {
  for (auto& node : nodes_)
   node.second->Release();
  nodes_.clear();
 }
 inline void Signal::Release() const {
  delete this;
 }
 inline void Signal::wait() {
  std::uint64_t total = 0;
  do {
   std::atomic_bool success = true;
   std::unique_lock<std::mutex> lock(*m_Mutex, std::defer_lock);
   lock.lock();
   for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
    switch (it->second->status()) {
    case SignalNodeStatus::activate: {
     if (it->second->timeout() > total)
      break;
     it->second->status(SignalNodeStatus::timeout);
    }break;
    case SignalNodeStatus::unknown: {
     it->second->status(SignalNodeStatus::activate);
    }break;
    case SignalNodeStatus::trigger: [[fallthrough]];
    case SignalNodeStatus::shutdown: [[fallthrough]];
    case SignalNodeStatus::timeout: {
     it->second->status(SignalNodeStatus::finish);
     it->second->OnSignalTrigger();
    }break;
    default:
     break;
    }
    if ((it->second->status() != SignalNodeStatus::finish) && success.load())
     success.store(false);
   }
   total += 100;
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   lock.unlock();
   if (success.load())
    break;
  } while (1);
 }
 inline SignalNode* Signal::search(const std::uint64_t&) const {
  SignalNode* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);

  return result;
 }
 inline SignalNode* Signal::create() {
  SignalNode* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  result = new SignalNode(nodes_.size() + 1);
  /*result->status(SignalNodeStatus::activate);*/
  return nodes_.emplace(result->identify(), result).first->second;
 }
 inline SignalNode* Signal::create(const std::uint64_t& timeout, const tfOnSignalTriggerCb& onSignalTriggerCb) {
  SignalNode* result = nullptr;
  std::lock_guard<std::mutex>(*m_Mutex);
  result = new SignalNode(nodes_.size() + 1);
  result->timeout(timeout);
  result->RegisterOnSignalTriggerCb(onSignalTriggerCb);
  return nodes_.emplace(result->identify(), result).first->second;
 }

}///namespace shared




/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 16 Jul 2023 01:11:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__843EC316_8803_45FA_9D47_714D645195CF__