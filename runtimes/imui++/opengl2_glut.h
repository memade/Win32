#if !defined(__33880904_0088_4E41_8385_0FD2D37D1935__)
#define __33880904_0088_4E41_8385_0FD2D37D1935__

#if IMGUI_GLUT_OPENGL2
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl2.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
namespace local {

 class OpenGL2GlutDrive final : public IDrive {
 public:
  OpenGL2GlutDrive(const IDearImGui*);
  virtual ~OpenGL2GlutDrive();
 public:
  bool Start() override final;
  void Stop() override final;
  void Release() const override final;
  void Process() override final;
 private:
  std::atomic_bool m_IsOpen = false;
 };


}///namespace local
#endif///IMGUI_GLUT_OPENGL2

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 25 Jun 2023 01:07:42 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__33880904_0088_4E41_8385_0FD2D37D1935__

