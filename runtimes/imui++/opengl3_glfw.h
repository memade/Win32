#if !defined(__EB1164E4_251A_4A98_AA47_DC1A62D7D63E__)
#define __EB1164E4_251A_4A98_AA47_DC1A62D7D63E__
#if IMGUI_GLFW_OPENGL3
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h>
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
namespace local {

 class OpenGL3GlfwDrive final : public IDrive {
 public:
  OpenGL3GlfwDrive(const IDearImGui*);
  virtual ~OpenGL3GlfwDrive();
 public:
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  void Process() override final;
 private:
  std::atomic_bool m_IsOpen = false;
 };


}///namespace local
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
#endif
/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:10:30 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__EB1164E4_251A_4A98_AA47_DC1A62D7D63E__