#if !defined(__49CC2C65_F19B_4977_8AD8_BF7A86BC9F55__)
#define __49CC2C65_F19B_4977_8AD8_BF7A86BC9F55__

#define IMGUI_GLUT_OPENGL2 0
#define IMGUI_GLFW_OPENGL2 0
#define IMGUI_GLFW_OPENGL3 0
#define IMGUI_WIN32_DIRECTX9 1
#define IMGUI_WIN32_DIRECTX10 0
#define IMGUI_WIN32_DIRECTX11 0
#define IMGUI_WIN32_DIRECTX12 0

#if IMGUI_WIN32_DIRECTX9 || IMGUI_WIN32_DIRECTX10 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX12
#define IMGUI_DIRECTX_ENABLE 1
#elif IMGUI_GLUT_OPENGL2 || IMGUI_GLFW_OPENGL2 || IMGUI_GLFW_OPENGL3
#define IMGUI_OPENGL_ENABLE 1
#elif
#define IMGUI_DEFAULT_ENABLE 1
#endif

#include <imgui.h>
#include <win.hpp>
#include <imui.hpp>
using namespace imui;
using namespace skin;

#include "drive.h"
#include "directx9_win32.h"
#include "directx10_win32.h"
#include "directx11_win32.h"
#include "directx12_win32.h"
#include "opengl2_glfw.h"
#include "opengl2_glut.h"
#include "opengl3_glfw.h"

#include "config.h"
#include "control.h"
#include "imui.h"
#include "export.h"


#pragma comment(lib,"imgui.lib")
/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 07:35:57 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__49CC2C65_F19B_4977_8AD8_BF7A86BC9F55__

