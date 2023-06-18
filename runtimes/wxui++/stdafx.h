#if !defined(__BD2E3E9F_0086_4128_8D8F_D87B2144CDA5__)
#define __BD2E3E9F_0086_4128_8D8F_D87B2144CDA5__

#include <win.hpp>
#include <wxui.hpp>
using namespace wxui;

#pragma warning(disable:4996)

#if !defined(_CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE  1
#endif
#if !defined(_CRT_NON_CONFORMING_SWPRINTFS)
#define _CRT_NON_CONFORMING_SWPRINTFS  1
#endif
#if !defined(_SCL_SECURE_NO_WARNINGS)
#define _SCL_SECURE_NO_WARNINGS  1
#endif
#if !defined(__WXMSW__)
#define __WXMSW__
#endif
#if !defined(NOPCH)
#define NOPCH
#endif

#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Rpcrt4.lib")

#if defined(_DEBUG)
#pragma comment(lib,"wxbase32ud.lib")
#pragma comment(lib,"wxmsw32ud_core.lib")
#pragma comment(lib,"wxmsw32ud_aui.lib")
#pragma comment(lib,"wxmsw32ud_html.lib")
#pragma comment(lib,"wxmsw32ud_core.lib")
#pragma comment(lib,"wxbase32ud_xml.lib")
#pragma comment(lib,"wxtiffd.lib")
#pragma comment(lib,"wxjpegd.lib")
#pragma comment(lib,"wxpngd.lib")
#pragma comment(lib,"wxzlibd.lib")
#pragma comment(lib,"wxregexud.lib")
#pragma comment(lib,"wxexpatd.lib")
#pragma comment(lib,"wxmsw32ud_gl.lib")
#pragma comment(lib,"wxmsw32ud_propgrid.lib")
#else
#pragma comment(lib,"wxbase32u.lib")
#pragma comment(lib,"wxmsw32u_core.lib")
#pragma comment(lib,"wxmsw32u_aui.lib")
#pragma comment(lib,"wxmsw32u_html.lib")
#pragma comment(lib,"wxmsw32u_core.lib")
#pragma comment(lib,"wxbase32u_xml.lib")
#pragma comment(lib,"wxtiff.lib")
#pragma comment(lib,"wxjpeg.lib")
#pragma comment(lib,"wxpng.lib")
#pragma comment(lib,"wxzlib.lib")
#pragma comment(lib,"wxregexu.lib")
#pragma comment(lib,"wxexpat.lib")
#pragma comment(lib,"wxmsw32u_gl.lib")
#pragma comment(lib,"wxmsw32u_propgrid.lib")
#endif
// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/wx.h"
#include "wx/nativewin.h"
#include "wx/mdi.h"
#include "wx/docmdi.h"
#include "wx/aui/aui.h"
#include "wx/app.h"
#include "wx/log.h"
#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/image.h"
#include "wx/button.h"
#include "wx/checkbox.h"
#include "wx/listbox.h"
#include "wx/statbox.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/msgdlg.h"
#include "wx/grid.h"
#include "wx/propgrid/manager.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/sysopt.h"
#include "wx/bookctrl.h"
#include "wx/treebook.h"
#include "wx/sizer.h"
#include "wx/fontdlg.h"
#include "wx/numdlg.h"
#include "wx/textdlg.h"
#include "wx/wupdlock.h"
#include "wx/textcompleter.h"
#include "wx/persist/toplevel.h"
#include "wx/persist/treebook.h"
#include "wx/listctrl.h"
#include "wx/glcanvas.h"
#include "wx/rawbmp.h"
#include "wx/popupwin.h"
#include "wx/spinctrl.h"
#include "wx/colordlg.h"
#include "wx/fontdlg.h"
#include "wx/aboutdlg.h"
#include "wx/odcombo.h"
#include "wx/dynlib.h"
#include "wx/msw/wrapwin.h"
#include "wx/dcbuffer.h"
#include "wx/mstream.h"
#include "wx/graphics.h"
#include "wx/notebook.h"
#include "wx/artprov.h"
#include "wx/creddlg.h"
#include "wx/filedlg.h"
#include "wx/cmdline.h"
#include "wx/notifmsg.h"
#include "wx/settings.h"
#include "wx/infobar.h"
#include "wx/filesys.h"
#include "wx/fs_arc.h"
#include "wx/fs_mem.h"
#include "wx/stdpaths.h"
#if wxUSE_STC
#include "wx/stc/stc.h"
#else
#include "wx/textctrl.h"
#endif

#if 1//!@ wxwebview
#if !wxUSE_WEBVIEW_WEBKIT && !wxUSE_WEBVIEW_WEBKIT2 && !wxUSE_WEBVIEW_IE && !wxUSE_WEBVIEW_EDGE
#error "A wxWebView backend is required by this sample"
#endif
#include "wx/webview.h"
#if wxUSE_WEBVIEW_IE
#include "wx/msw/webview_ie.h"
#endif
#if wxUSE_WEBVIEW_EDGE
#include "wx/msw/webview_edge.h"
#endif
#include "wx/webviewarchivehandler.h"
#include "wx/webviewfshandler.h"
#endif


#include "config.h"
#include "wxframeweb.h"
#include "wxchild.h"
#include "wxframe.h"
#include "wxapp.h"
#include "wxui.h"
#include "export.h"

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 19 May 2023 10:24:47 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__BD2E3E9F_0086_4128_8D8F_D87B2144CDA5__
