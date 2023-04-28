#include <win.hpp>
#include <libuv.hpp>

#if 1
int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 auto server_tcp_ = libuv::IServer::CreateInterface(
  R"(D:\__Github__\Win32\libs\runtimes\x64\Debug\libuv.dll)",
  "api_server_init",
  "api_server_uninit"
  );
 auto config = server_tcp_->ConfigGet();
 //config->ServerType(libuv::EnServerType::TCP);
 config->ServerType(libuv::EnServerType::PIPE);
 config->IPAddrV4("0.0.0.0");
 config->Port(8888);
 server_tcp_->Start();

 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    server_tcp_->Stop();
    libuv::IServer::DestoryInterface(server_tcp_);
    exit = true;
   }
  });
 return 0;
}
#else


#if 0
#define NUM_WRITE_REQS 32

static uv_tcp_t tcp_handle;
static uv_connect_t connect_req;

static void connect_cb(uv_connect_t* req, int status);
static void write_cb(uv_write_t* req, int status);
static void close_cb(uv_handle_t* handle);

static void connect_cb(uv_connect_t* conn_req, int status) {
 uv_write_t* req;
 uv_buf_t buf;
 int i, r;
 char ping_c[] = "PING";
 buf = uv_buf_init(ping_c, 4);
 for (i = 0; i < NUM_WRITE_REQS; i++) {
  req = (uv_write_t*)malloc(sizeof * req);

  r = uv_write(req, (uv_stream_t*)&tcp_handle, &buf, 1, write_cb);
 }
 uv_close((uv_handle_t*)&tcp_handle, close_cb);
}

static void write_cb(uv_write_t* req, int status) {
 /* write callbacks should run before the close callback */
 //ASSERT(req->handle == (uv_stream_t*)&tcp_handle);
 free(req);
}

static void close_cb(uv_handle_t* handle) {
 //ASSERT(handle == (uv_handle_t*)&tcp_handle);
}

static void connection_cb(uv_stream_t* server, int status) {
}

static void start_server(uv_loop_t* loop, uv_tcp_t* handle) {
 struct sockaddr_in addr;
 int r;
 uv_ip4_addr("127.0.0.1", 9123, &addr);
 r = uv_tcp_init(loop, handle);
 r = uv_tcp_bind(handle, (const struct sockaddr*)&addr, 0);
 r = uv_listen((uv_stream_t*)handle, 128, connection_cb);
 uv_unref((uv_handle_t*)handle);
}

int main(int argc, char** args) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(263);
#endif

 struct sockaddr_in addr;
 uv_tcp_t tcp_server;
 uv_loop_t* loop;
 int r;
 uv_ip4_addr("127.0.0.1", 9123, &addr);
 loop = uv_default_loop();
 /* We can't use the echo server, it doesn't handle ECONNRESET. */
 start_server(loop, &tcp_server);
 r = uv_tcp_init(loop, &tcp_handle);
 r = uv_tcp_connect(&connect_req,
  &tcp_handle,
  (const struct sockaddr*)&addr,
  connect_cb);
 r = uv_run(loop, UV_RUN_DEFAULT);
 //MAKE_VALGRIND_HAPPY();
 libuv::uv_shutdown();
 std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 return 0;


#if 0
 Server server_;

 server_.Start();
 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {
   if (input == "q") {
    server_.Stop();
    exit = true;
   }
  });
#endif

 return 0;
}
#endif
#endif