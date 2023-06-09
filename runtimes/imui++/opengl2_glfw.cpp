﻿#include "stdafx.h"

#if IMGUI_GLFW_OPENGL2
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
namespace local {


 OpenGL2GlfwDrive::OpenGL2GlfwDrive(const IDearImGui* host, const Control* ctrl) : IDrive(host, ctrl) {

 }
 OpenGL2GlfwDrive::~OpenGL2GlfwDrive() {

 }
 void OpenGL2GlfwDrive::Release() const {
  delete this;
 }
 bool OpenGL2GlfwDrive::Create() {
  bool result = false;
  do {
   glfwSetErrorCallback([](int, const char*) {});
   if (!glfwInit())
    break;
   // Create window with graphics context
   m_pGLFWWindow = glfwCreateWindow(
    m_Size.GetCX(), 
    m_Size.GetCY(), 
    "Dear ImGui GLFW+OpenGL2 example", NULL, NULL);
   result = m_pGLFWWindow!=nullptr;
  } while (0);
  return result;
 }
 void OpenGL2GlfwDrive::Destroy() {
  do {
   if (!m_pGLFWWindow)
    break;
   // Cleanup
   ImGui_ImplOpenGL2_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   glfwDestroyWindow(m_pGLFWWindow);
   glfwTerminate();
  } while (0);
 }
 bool OpenGL2GlfwDrive::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Process(); });
  } while (0);
  return m_IsOpen.load();
 }
 void OpenGL2GlfwDrive::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }

 void OpenGL2GlfwDrive::Process() {

  if (!Create())
   return;

  glfwMakeContextCurrent(m_pGLFWWindow);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_pGLFWWindow, true);
  ImGui_ImplOpenGL2_Init();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(m_pGLFWWindow)) {
   // Poll and handle events (inputs, window resize, etc.)
   // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
   // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
   // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
   // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
   glfwPollEvents();
   // Start the Dear ImGui frame
   ImGui_ImplOpenGL2_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();
#if 1
   OnRender();
#else
   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

   // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
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

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
   }

   // 3. Show another simple window.
   if (show_another_window)
   {
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
   int display_w, display_h;
   glfwGetFramebufferSize(m_pGLFWWindow, &display_w, &display_h);
   SetSize(Vec2(display_w, display_h));
   glViewport(0, 0, display_w, display_h);
   glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   glClear(GL_COLOR_BUFFER_BIT);

   // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
   // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
   //GLint last_program;
   //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
   //glUseProgram(0);
   ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
   //glUseProgram(last_program);

   glfwMakeContextCurrent(m_pGLFWWindow);
   glfwSwapBuffers(m_pGLFWWindow);
  }

  Destroy();
 }


}///namespace local
#endif
