#include "stdafx.h"

namespace wx {
 Theme::Theme(const IdentifyTheme& identify, const std::string& shape_img_pathname, const std::string& bgk_img_pathname)
  : m_Identify(identify) {
  do {
   if (m_Identify.empty())
    break;
   if (!From(bgk_img_pathname, &m_pMImgBgk, &m_pImgBgk))
    break;
   if (!From(shape_img_pathname, &m_pMImgShape, &m_pImgShape))
    break;
   m_MainWindowSize = m_pImgShape->GetSize();
   m_Ready.store(true);
  } while (0);
 }
 Theme::~Theme() {
  if (m_pImgBgk) {
   delete m_pImgBgk;
   m_pImgBgk = nullptr;
  }
  if (m_pImgShape) {
   delete m_pImgShape;
   m_pImgShape = nullptr;
  }
  if (m_pMImgBgk) {
   delete m_pMImgBgk;
   m_pMImgBgk = nullptr;
  }
  if (m_pMImgShape) {
   delete m_pMImgShape;
   m_pMImgShape = nullptr;
  }
 }
 const DWORD& Theme::ColorBgk() const {
  return m_BgkColor;
 }
 const wxSize& Theme::MainWindowSize() const {
  return m_MainWindowSize;
 }
 const wxImage& Theme::ImgShape() const {
  return *m_pImgShape;
 }
 const wxImage& Theme::ImgBgk() const {
  return *m_pImgBgk;
 }
 const IdentifyTheme& Theme::Identify() const {
  return m_Identify;
 }
 bool Theme::Ready() const {
  return m_Ready.load();
 }
 bool Theme::From(const std::string& imgPathname, wxMemoryInputStream** ppMInputStream, wxImage** ppImage) {
  bool result = false;
  *ppImage = nullptr;
  *ppMInputStream = nullptr;
  do {
   do {
    auto inputBuffer = shared::Win::File::ReadCXX(imgPathname);
    if (inputBuffer.empty())
     break;
    *ppMInputStream = new wxMemoryInputStream(inputBuffer.data(), inputBuffer.size());
    result = true;
    shared::ImageType imgType = shared::Win::GetImageTypeByMemory(inputBuffer);
    switch (imgType) {
    case shared::ImageType::_PNG: {
     *ppImage = new wxImage(**ppMInputStream, wxBITMAP_TYPE_PNG);
     break;
    }break;
    case shared::ImageType::_BMP: {
     *ppImage = new wxImage(**ppMInputStream, wxBITMAP_TYPE_BMP);
    }break;
    case shared::ImageType::_GIF: {
     *ppImage = new wxImage(**ppMInputStream, wxBITMAP_TYPE_GIF);
    }break;
    case shared::ImageType::_ICO: {
     *ppImage = new wxImage(**ppMInputStream, wxBITMAP_TYPE_ICO);
    }break;
    case shared::ImageType::_JPEG: {
     *ppImage = new wxImage(**ppMInputStream, wxBITMAP_TYPE_JPEG);
    }break;
    default: {
     result = false;
    }break;
    }
   } while (0);
   if (result) {
    result = (*ppImage)->IsOk();
   }
   if (!result) {
    if (*ppImage) {
     delete *ppImage;
     *ppImage = nullptr;
    }
    if (*ppMInputStream) {
     delete *ppMInputStream;
     *ppMInputStream = nullptr;
    }
   }
  } while (0);
  return result;
 }

 extern const int WX_CMD_ONAPPCREATEFRAME = wxNewId();
 extern const int WX_CMD_ONAPPDESTORY = wxNewId();
 extern const int WX_CMD_SHOWWINDOW = wxNewId();
}///namespace wx