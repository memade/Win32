#if !defined(__C02B61A2_34BB_471E_A126_9C98F5969413__)
#define __C02B61A2_34BB_471E_A126_9C98F5969413__


#define IMGUI_GLUT_OPENGL2 0
#define IMGUI_GLFW_OPENGL3 0
#define IMGUI_GLFW_OPENGL2 0
#define IMGUI_WIN32_DIRECTX9 0
#define IMGUI_WIN32_DIRECTX10 0
#define IMGUI_WIN32_DIRECTX11 0
#define IMGUI_WIN32_DIRECTX12 1
#include <imgui.h>

#if IMGUI_GLUT_OPENGL2
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl2.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#elif IMGUI_GLFW_OPENGL2
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
#elif IMGUI_GLFW_OPENGL3
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw.lib")
#pragma comment(lib,"opengl32.lib")
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#elif IMGUI_WIN32_DIRECTX9
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#elif IMGUI_WIN32_DIRECTX10
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_win32.h>
#include <d3d10_1.h>
#include <d3d10.h>
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#elif IMGUI_WIN32_DIRECTX11
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#elif IMGUI_WIN32_DIRECTX12
#define NUM_FRAMES_IN_FLIGHT 3
#define NUM_BACK_BUFFERS 3
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#if _DEBUG
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
typedef struct tagFrameContext
{
 ID3D12CommandAllocator* CommandAllocator;
 UINT64 FenceValue;
 tagFrameContext() { memset(this, 0x00, sizeof(*this)); }
 void operator=(const tagFrameContext& obj) { memcpy(this, &obj, sizeof(*this)); }
}FrameContext;
#endif

#if IMGUI_WIN32_DIRECTX12 || IMGUI_WIN32_DIRECTX11 || IMGUI_WIN32_DIRECTX10 || IMGUI_WIN32_DIRECTX9
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif





/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 27 May 2023 08:19:59 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__C02B61A2_34BB_471E_A126_9C98F5969413__