#include "stdafx.h"
#if IMGUI_GLUT_OPENGL2
namespace local {

 static bool show_demo_window = true;
 static bool show_another_window = false;
 static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 OpenGL2GlutDrive::OpenGL2GlutDrive(const IDearImGui* host) : IDrive(host) {

 }
 OpenGL2GlutDrive::~OpenGL2GlutDrive() {

 }
 void OpenGL2GlutDrive::Release() const {
  delete this;
 }
 bool OpenGL2GlutDrive::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void OpenGL2GlutDrive::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }

 void OpenGL2GlutDrive::Process() {
  // Create GLUT window
  glutInit(&__argc, __argv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(1024, 768);
  if (1 != glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example"))
   return;
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  // Setup Platform/Renderer backends
  // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
  ImGui_ImplGLUT_Init();
  ImGui_ImplOpenGL2_Init();
  ImGui_ImplGLUT_InstallFuncs();

  glutDisplayFunc(
   []() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();

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
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
    glutPostRedisplay();
   });

  glutMainLoop();

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGLUT_Shutdown();
  ImGui::DestroyContext();
 }



}///namespace local
#endif
