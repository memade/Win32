#include "stdafx.h"
#if IMGUI_GLUT_OPENGL2
namespace local {

 static bool show_demo_window = true;
 static bool show_another_window = false;
 static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 OpenGL2GlutDrive::OpenGL2GlutDrive(const IDearImGui* host, const Control* ui_ctrl) : IDrive(host, ui_ctrl) {

 }
 OpenGL2GlutDrive::~OpenGL2GlutDrive() {

 }
 void OpenGL2GlutDrive::Destroy() {
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGLUT_Shutdown();
  ImGui::DestroyContext();
 }
 bool OpenGL2GlutDrive::Create() {
  bool result = false;
  do {
   // Create GLUT window
   glutInit(&__argc, __argv);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
   //glutInitWindowSize(m_Size.GetCX(), m_Size.GetCY());
   if (1 != glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example"))
    break;
   glutReshapeWindow(m_Size.GetCX(), m_Size.GetCY());
   CreateNotify();
   result = true;
  } while (0);
  return result;
 }
 void OpenGL2GlutDrive::Release() const {
  delete this;
 }

 void OpenGL2GlutDrive::Process() {
#if 0
  // Create GLUT window
  glutInit(&__argc, __argv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(1024, 768);
  if (1 != glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example"))
   return;
#else
  if (!Create())
   return;
#endif
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  // Setup Platform/Renderer backends
  // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
  ImGui_ImplGLUT_Init();
  ImGui_ImplOpenGL2_Init();
  ImGui_ImplGLUT_InstallFuncs();
  ImGui::GetIO().UserData = this;
  glutDisplayFunc(
   []() {
    OpenGL2GlutDrive* pThis = reinterpret_cast<OpenGL2GlutDrive*>(ImGui::GetIO().UserData);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();
#if 1
    pThis->OnRender();
#else
    if (show_demo_window)
     ImGui::ShowDemoWindow(&show_demo_window);
    {
     static float f = 0.0f;
     static int counter = 0;
     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
     ImGui::Checkbox("Another Window", &show_another_window);
     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      counter++;
     ImGui::SameLine();
     ImGui::Text("counter = %d", counter);
     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
     ImGui::End();
    }
    if (show_another_window) {
     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
     ImGui::Text("Hello from another window!");
     if (ImGui::Button("Close Me"))
      show_another_window = false;
     ImGui::End();
    }
#endif
    ImGui::EndFrame();
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    void* udata = ImGui::GetIO().UserData;
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    //m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, static_cast<D3DCOLOR>(m_dwMainBkColor), 1.0f, 0);
    glClearColor(
     pThis->m_rgbaMainBkColor.x * pThis->m_rgbaMainBkColor.w,
     pThis->m_rgbaMainBkColor.y * pThis->m_rgbaMainBkColor.w,
     pThis->m_rgbaMainBkColor.z * pThis->m_rgbaMainBkColor.w,
     pThis->m_rgbaMainBkColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
    glutPostRedisplay();
   });

  glutReshapeFunc(
   [](int width, int height) {
    ImGui::GetIO().DisplaySize.x = width;
    ImGui::GetIO().DisplaySize.y = height;
    OpenGL2GlutDrive* pThis = reinterpret_cast<OpenGL2GlutDrive*>(ImGui::GetIO().UserData);
    pThis->SetSize(Vec2(width, height));
   });

  glutMainLoop();

  Destroy();
 }



}///namespace local
#endif
