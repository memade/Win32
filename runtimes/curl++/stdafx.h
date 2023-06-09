﻿#if !defined(INC_H___6CDDAE11_F6FE_44AD_A8A4_451272E21572__HEAD__)
#define INC_H___6CDDAE11_F6FE_44AD_A8A4_451272E21572__HEAD__

#include <win.hpp>
#include <zip++.h>
#include <http_parser.hpp>

#define ENABLE_OPENSSL_3 1

#define CURL_STATICLIB
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"normaliz.lib")

#if ENABLE_OPENSSL_3
//!@ Openssl 3.0
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#if 0
#pragma comment(lib,"libfips.lib")
#pragma comment(lib,"fips.lib")
#pragma comment(lib,"legacy.lib")
#pragma comment(lib,"libapps.lib")
#pragma comment(lib,"libcommon.lib")
#pragma comment(lib,"libdefault.lib")
#pragma comment(lib,"liblegacy.lib")
#pragma comment(lib,"libtestutil.lib")
#endif
#else
//!@ Openssl 1.0.2
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#endif

//!@ Libcurl 7.85.0
#define CURL_STATICLIB
#pragma comment(lib,"libcurl_zlib_ssl3-7.85.0.lib")

//!@ Curlpp 0.8.1
#define CURLPP_STATICLIB
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Multi.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#pragma comment(lib,"curlpp-0.8.1.lib")


///////////////////////////////////////////////////////////////////////////////////////////
#include <curl.hpp>
using namespace curl;

#include "config.h"
#include "cache.h"
#include "headers.h"
#include "request.h"
#include "response.h"
#include "curl++.h"


#include "export.h"

/// /*新生®（上海）**/
/// /*2022_09_07T01:06:39.5877592Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___6CDDAE11_F6FE_44AD_A8A4_451272E21572__HEAD__

