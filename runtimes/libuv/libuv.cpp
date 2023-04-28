#include "stdafx.h"

namespace local {

 ILibuv::ILibuv() {
#if 0
  uv_loop_t_ = (uv_loop_t*)::malloc(sizeof uv_loop_t);
  uv_tcp_server_ = (uv_tcp_t*)::malloc(sizeof uv_tcp_server_);
  uv_udp_server_ = (uv_udp_t*)::malloc(sizeof uv_udp_server_);
  uv_pipe_server_ = (uv_pipe_t*)::malloc(sizeof uv_pipe_server_);
  uv_connect_t_ = (uv_connect_t*)::malloc(sizeof uv_connect_t_);
#endif
 }

 ILibuv::~ILibuv() {
#if 0
  SK_DELETE_PTR_C(uv_loop_t_);
  SK_DELETE_PTR_C(uv_tcp_server_);
  SK_DELETE_PTR_C(uv_udp_server_);
  SK_DELETE_PTR_C(uv_pipe_server_);
  SK_DELETE_PTR_C(uv_connect_t_);
#endif
 }

 bool ILibuv::uv_get_ipaddr(const sockaddr_storage* in_addr, std::string& out_ip, u_short& out_port, const UVIPVType& ipv) {
  bool result = false;
  out_ip.clear();
  out_port = 0;
  if (!in_addr)
   return result;
  int inet = (UVIPVType::IPV6 == ipv) ? AF_INET6 : AF_INET;
  char ipv6[64] = { 0 };
  switch (inet) {
  case AF_INET6: {
   struct sockaddr_in6* addr6 = (struct sockaddr_in6*)in_addr;
   //低版本windows可能找不到inet_ntop函数。
#if _MSC_VER
   DWORD size = sizeof(ipv6);
   ::WSAAddressToStringA((LPSOCKADDR)addr6, sizeof(sockaddr_in6), NULL, ipv6, &size);
   out_ip = std::string(ipv6);
   auto index = out_ip.rfind(":");
   if (index >= 0) {
    out_ip.resize(index);
   }
   out_port = ::htons(addr6->sin6_port);
#else
   std::string str(::inet_ntop(inet, (void*)&(addr6->sin6_addr), ipv6, 64));
   out_ip.swap(str);
   out_port = ::htons(addr6->sin6_port);
#endif
  }break;
  case AF_INET: {
   struct sockaddr_in* addr4 = (struct sockaddr_in*)in_addr;
   std::string str(::inet_ntoa(addr4->sin_addr));
   out_ip.swap(str);
   out_port = ::htons(addr4->sin_port);
  }break;
  default:
   result = false;
   break;
  }
  return result;
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
#endif
 void ILibuv::uv_close_loop(uv_loop_t* loop_) {
  do {
   if (!loop_)
    break;
   uv_walk(loop_,
    [](uv_handle_t* handle, void* arg) {
     if (!uv_is_closing(handle))
      uv_close(handle, NULL);
    }, NULL);
   uv_run(loop_, UV_RUN_DEFAULT);
   uv_loop_close(loop_);
  } while (0);

  auto default_loop_ = uv_default_loop();
  do {
   if (!default_loop_)
    break;
   uv_walk(default_loop_,
    [](uv_handle_t* handle, void* arg) {
     if (!uv_is_closing(handle))
      uv_close(handle, NULL);
    }, NULL);
   uv_run(default_loop_, UV_RUN_DEFAULT);
   uv_loop_close(default_loop_);
  } while (0);
  uv_library_shutdown();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
 void ILibuv::uv_close_stream(uv_stream_t* stream) {
  uv_shutdown_t* sreq = new uv_shutdown_t;
  if (uv_is_writable(stream))
   uv_shutdown(sreq, stream,
    [](uv_shutdown_t* req, int status) {
     if (status == 0) {
      uv_close((uv_handle_t*)req->handle, NULL);
      delete req;
      req = nullptr;
     }
    });
  else {
   delete sreq;
   sreq = nullptr;
  }
 }




#if 0
#if 0
 uv_shutdown_t* sreq = new uv_shutdown_t;
 // if (uv_is_writable((uv_stream_t*)uv_tcp_t_)) 
 int shutdown_ret = uv_shutdown(sreq, (uv_stream_t*)uv_tcp_t_,
  [](uv_shutdown_t* req, int status) {
   if (status == 0) {
    uv_close((uv_handle_t*)req->handle,
     [](uv_handle_t* peer) {
      auto pSession = (Session*)peer->data;
      pSession->closed_ = true;

      if (uv_is_active((uv_handle_t*)pSession->uv_async_t_)) {
       async_req_t* req = new async_req_t;
       req->host = pSession;
       req->req_type = async_req_type::Close;
       pSession->uv_async_t_->data = req;
       uv_async_send(pSession->uv_async_t_);
      }

     });
    SK_DELETE_PTR(req);
   }

  });

 if (0 != shutdown_ret) {
  uv_close((uv_handle_t*)uv_tcp_t_,
   [](uv_handle_t* handle) {
    auto sk = 0;
   });
  SK_DELETE_PTR(sreq);
 }
#endif
#endif
}///namespace local
