#if !defined(__751FACCB_F8E0_4C8A_AFC5_72AB4981084B__)
#define __751FACCB_F8E0_4C8A_AFC5_72AB4981084B__

#include <win.hpp>
#include <cef.hpp>
using namespace cef;

#ifdef IsMinimized
#undef IsMinimized
#endif

#ifdef GetNextSibling
#undef GetNextSibling
#endif

#ifdef GetFirstChild
#undef GetFirstChild
#endif

#ifdef IsMaximized
#undef IsMaximized
#endif


#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/wrapper/cef_helpers.h>
#include <include/cef_command_line.h>

#include <include/cef_sandbox_win.h>


#include "config.h"
#include "cef++.h"

#include "export.h"



#pragma comment(lib,"libcef.lib")
#pragma comment(lib,"libcef_dll_wrapper.lib")
#if !_DEBUG
#pragma comment(lib,"cef_sandbox.lib")
#if CEF_USE_SANDBOX
#endif
#endif
/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 03 Apr 2023 08:28:02 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__751FACCB_F8E0_4C8A_AFC5_72AB4981084B__



