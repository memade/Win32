#include "stdafx.h"

namespace shared {

 bool Win::Encryption::MD5(const std::string& input, std::string& output) {
  bool result = false;
  output.clear();
  HCRYPTPROV hProv = NULL;
  HCRYPTPROV hHash = NULL;
  do {
   if (::CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)
    break;
   if (::CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)
    break;
   if (::CryptHashData(hHash, (BYTE*)input.data(), static_cast<DWORD>(input.size()), NULL) == FALSE)
    break;
   DWORD dwHashLen = sizeof(DWORD);
   if (::CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0) == FALSE)
    break;
   output.resize(dwHashLen, 0x00);
   if (::CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)&output[0], &dwHashLen, 0) == FALSE)
    break;
   result = true;
  } while (0);
  if (hHash) {
   if (TRUE == ::CryptDestroyHash(hHash))
    hHash = NULL;
  }
  if (hProv) {
   if (TRUE == ::CryptReleaseContext(hProv, 0))
    hProv = NULL;
  }
  return result;
 }
 std::string Win::Encryption::MD5(const std::string& input, const bool& lower /*= true*/) {
  std::string result;
  do {
   if (!MD5(input, result) || result.empty())
    break;
   result = shared::Win::BinToHex(result);
   if (!lower)
    result = IConv::ToUpperA(result);
  } while (0);
  return result;
 }

 std::string Win::Encryption::WemadeEncode(const std::string& input) {
  std::string result;
  size_t nSrcLen = input.length();
  std::uint8_t* pszSrc = (std::uint8_t*)input.c_str();
  int nDestPos = 0;
  int nRestCount = 0;
  std::uint8_t	chMade = 0;
  std::uint8_t chRest = 0;
  for (size_t i = 0; i < nSrcLen; i++) {
   chMade = ((chRest | (pszSrc[i] >> (2 + nRestCount))) & 0x3f);
   chRest = (((pszSrc[i] << (8 - (2 + nRestCount))) >> 2) & 0x3f);
   nRestCount += 2;
   if (nRestCount < 6) {
    result.push_back(chMade + 0x3c);
   }
   else {
    result.push_back(chMade + 0x3c);
    result.push_back(chRest + 0x3c);
    nRestCount = 0;
    chRest = 0;
   }
  }
  if (nRestCount > 0) {
   result.push_back(chRest + 0x3c);
  }
  if (!result.empty()) {
   result.insert(0, sizeof(char), 0x23);
   result.push_back(0x21);
  }
  return result;
 }

 std::string Win::Encryption::WemadeDecode(const std::string& input) {
  std::string result;
  std::string strSrc = input;
  if (strSrc.empty()) {
   return result;
  }
  size_t endPos = strSrc.rfind(0x21);
  if (endPos == std::string::npos) {
   return result;
  }
  else {
   strSrc.erase(endPos, strSrc.size() - endPos);
   strSrc.push_back(0x21);
  }
  if (0x23 != strSrc.front() || 0x21 != strSrc.back()) {
   return result;
  }
  strSrc.erase(strSrc.begin());
  strSrc.pop_back();
  const std::uint8_t Decode6BitMask[5] = { 0xfc, 0xf8, 0xf0, 0xe0, 0xc0 };
  int nDestPos = 0;
  int nBitPos = 2;
  int nMadeBit = 0;
  std::uint8_t	 ch = 0;
  std::uint8_t chCode = 0;
  std::uint8_t tmp = 0;
  for (auto it = strSrc.begin(); it != strSrc.end(); ++it) {
   if ((*it - 0x3c) >= 0) {
    ch = *it - 0x3c;
   }
   else {
    nDestPos = 0;
    break;
   }
   if ((nMadeBit + 6) >= 8) {
    chCode = (tmp | ((ch & 0x3f) >> (6 - nBitPos)));
    result.push_back(chCode);
    nMadeBit = 0;
    if (nBitPos < 6)
     nBitPos += 2;
    else {
     nBitPos = 2;
     continue;
    }
   }
   tmp = ((ch << nBitPos) & Decode6BitMask[nBitPos - 2]);
   nMadeBit += (8 - nBitPos);
  }
  return result;
 }
}///namespace 