#if !defined(__1E2DDCE8_5CFD_4D2A_BF7C_E08CCA2465C1__)
#define __1E2DDCE8_5CFD_4D2A_BF7C_E08CCA2465C1__

#if defined(_WIN32)
#define notify_parent_process() ((void) 0)
#else
extern void notify_parent_process(void);
#endif

namespace local {

 class ILibuv {
 public:
  ILibuv();
  ~ILibuv();
 protected:
  uv_loop_t* uv_loop_t_ = nullptr;
  uv_connect_t* uv_connect_t_ = nullptr;
  uv_stream_t* uv_server_ = nullptr;
  uv_stream_t* uv_client_ = nullptr;
  uv_async_t* uv_async_t_ = nullptr;
  uv_thread_t* uv_thread_main_ = nullptr;
 public: 
  /* This macro cleans up the main loop. This is used to avoid valgrind
   * warnings about memory being "leaked" by the main event loop.
   */
  static void uv_close_loop(uv_loop_t*);
  static void uv_close_stream(uv_stream_t*);
  static bool uv_get_ipaddr(const sockaddr_storage* in_addr, std::string& out_ip, u_short& out_port, const UVIPVType& ipv = UVIPVType::IPV4);
 };


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 22 Apr 2023 04:21:58 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__1E2DDCE8_5CFD_4D2A_BF7C_E08CCA2465C1__

