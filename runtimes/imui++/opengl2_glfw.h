#if !defined(__C0D40904_7123_43F8_A544_25A8B650503B__)
#define __C0D40904_7123_43F8_A544_25A8B650503B__

#if IMGUI_GLFW_OPENGL2
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <GLFW/glfw3.h>
namespace local {


 class OpenGL2GlfwDrive : public IDrive {
 public:
  OpenGL2GlfwDrive(const IDearImGui*, const Control*);
  virtual ~OpenGL2GlfwDrive();
 public:
  bool Start() override;
  void Stop() override;
  void Release() const override;
  void Process() override;
 protected:
  bool Create() override;
  void Destroy() override;
 private:
  GLFWwindow* m_pGLFWWindow = nullptr;
  std::atomic_bool m_IsOpen = false;
 };

}///namespace local
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
#endif

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:10:01 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C0D40904_7123_43F8_A544_25A8B650503B__