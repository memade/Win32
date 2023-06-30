#include "stdafx.h"

namespace local {
 static bool show_another_window = false;

 IDrive::IDrive(const IDearImGui* host) : m_pHost(host) {

 }

 const HWND& IDrive::Handle() const {
  return m_hWnd;
 }
 bool IDrive::OnCreateApplicationWindow(IDrive* pCreateHost, const WNDPROC& wndProc) {
  bool result = false;
  const skin::INodeRender* pSkinStyle = nullptr;
  do {//!@ skin . . .
   if (!Host())
    break;
   pSkinStyle = Host()->SkinGet()->MainNode();
   if (!pSkinStyle)
    break;

   m_WndClassEx.cbSize = sizeof(m_WndClassEx);
   m_WndClassEx.style = CS_CLASSDC;
   m_WndClassEx.lpfnWndProc = wndProc;
   m_WndClassEx.cbClsExtra = 0L;
   m_WndClassEx.cbWndExtra = 0L;
   m_WndClassEx.hInstance = __gpHinstance;
   m_WndClassEx.hIcon = pSkinStyle->LogoIcon();
   m_WndClassEx.hCursor = NULL;
   m_WndClassEx.hbrBackground = NULL;
   m_WndClassEx.lpszMenuName = NULL;
   m_WndClassEx.lpszClassName = pSkinStyle->ClassName()? pSkinStyle->ClassName()->wstr() : LR"(9E827EBF7EC6)";
   m_WndClassEx.hIconSm = pSkinStyle->LogoIcon();

   void(::RegisterClassExW(&m_WndClassEx));

   m_hWnd = ::CreateWindowW(
    m_WndClassEx.lpszClassName,
    pSkinStyle->Text()->wstr(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    pSkinStyle->Size()->cx,
    pSkinStyle->Size()->cy,
    NULL,
    NULL,
    m_WndClassEx.hInstance,
    pCreateHost);

   if (!m_hWnd)
    break;

   Host()->SkinGet()->MainNode()->Handle(m_hWnd);

   ::ShowWindow(m_hWnd, pSkinStyle->Visible() ? SW_SHOWDEFAULT : SW_HIDE);

   result = true;
  } while (0);

  do {
   if (pSkinStyle)
    break;

   m_WndClassEx.cbSize = sizeof(m_WndClassEx);
   m_WndClassEx.style = CS_CLASSDC;
   m_WndClassEx.lpfnWndProc = wndProc;
   m_WndClassEx.cbClsExtra = 0L;
   m_WndClassEx.cbWndExtra = 0L;
   m_WndClassEx.hInstance = __gpHinstance;
   m_WndClassEx.hIcon = NULL;
   m_WndClassEx.hCursor = NULL;
   m_WndClassEx.hbrBackground = NULL;
   m_WndClassEx.lpszMenuName = NULL;
   m_WndClassEx.lpszClassName = LR"(ImGui Example)";
   m_WndClassEx.hIconSm = NULL;
   void(::RegisterClassExW(&m_WndClassEx));
   m_hWnd = ::CreateWindowW(
    m_WndClassEx.lpszClassName,
    L"Dear ImGui DirectX9 Example",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    NULL,
    NULL,
    m_WndClassEx.hInstance,
    pCreateHost);
   if (!m_hWnd)
    break;
   ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
   result = true;
  } while (0);

  Host()->Center();
  return result;
 }

 void IDrive::OnRefreshBkcolor(DWORD& outClearColorRgba) {
  do {
   auto pSkinStyle = Host()->SkinGet()->MainNode();
   if (!pSkinStyle)
    break;
   std::uint8_t r = 0;
   std::uint8_t g = 0;
   std::uint8_t b = 0;
   std::uint8_t a = 0;
   Host()->SkinGet()->ParserArgb(pSkinStyle->BkColor(), a, r, g, b);
   float rf = r / 255.0;
   float gf = g / 255.0;
   float bf = b / 255.0;
   float af = a / 255.0;
   outClearColorRgba = D3DCOLOR_RGBA((int)(rf * af * 255.0f), (int)(gf * af * 255.0f), (int)(bf * af * 255.0f), (int)(af * 255.0f));
  } while (0);
 }
 void IDrive::OnLoadFonts() {
  do {
   if (!Host())
    break;
   if (!Host()->SkinGet())
    break;
   Host()->SkinGet()->FontNodeIterator(
    [](skin::INodeRender* pFontNode) {
     do {
      if (pFontNode->FlagInitialization())
       break;
      pFontNode->FlagInitialization(true);
      std::string font_cache;
      pFontNode->FontCache(
       [&font_cache](const char* buffer, const size_t& len) {
        font_cache.append((char*)buffer, len);
       });
      if (font_cache.empty())
       break;
#if 1
      ImFontConfig font_cfg;
      font_cfg.FontDataOwnedByAtlas = false;
      auto pFonts = ImGui::GetIO().Fonts;
      ImFont* pFont = pFonts->AddFontFromMemoryTTF(\
       font_cache.data(), static_cast<int>(font_cache.size()), pFontNode->Pixels(), &font_cfg, pFonts->GetGlyphRangesChineseSimplifiedCommon());
#else
      ImFont* pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(pFontNode->FontFilePathname(), pFontNode->Pixels());
#endif
      if (!pFont)
       break;

      auto sksks = 0;
     } while (0);
    });

   /*ImGui::GetIO().Fonts->AddFontDefault();*/
  } while (0);
 }
 bool IDrive::OnRender() {
  bool result = false;
  do {
#if 0
   ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
   ImGui::Text("Hello from another window!");
   if (ImGui::Button("Close Me"))
    show_another_window = false;
   ImGui::End();
#endif

   Host()->SkinGet()->NodeIterator(
    [&](skin::INodeRender* pINode) {//!@ begin
     do {
      auto ctrl = reinterpret_cast<ICtrl*>(pINode->Route());
      if (!ctrl)
       break;
      ctrl->Begin();
     } while (0);
    },
    [&](skin::INodeRender* pINode) {//!@ end
     do {
      auto ctrl = reinterpret_cast<ICtrl*>(pINode->Route());
      if (!ctrl)
       break;
      ctrl->End();
     } while (0);
    });

#if 0

#endif
   //ImGui::BeginChildFrame(1, ImVec2(100, 100), true);
   //ImGui::BeginChildFrame(2, ImVec2(50, 50), true);
   //ImGui::BeginChildFrame(3, ImVec2(40, 40), true);
   //ImGui::BeginChildFrame(4, ImVec2(30, 30), true);
   //ImGui::BeginChildFrame(5, ImVec2(20, 20), true);
   //ImGui::BeginChildFrame(6, ImVec2(10, 10), true);
   //ImGui::EndChildFrame();
   //ImGui::EndChildFrame();
   //ImGui::EndChildFrame();
   //ImGui::EndChildFrame();
   //ImGui::EndChildFrame();
#if 0
   int index = 1;
   Host()->SkinGet()->NodeIterator(
    [&](skin::INode* pINode, const bool& end_signal) {

     //ImGui::BeginChildFrame(index++, ImVec2(100, 100), true);
     //ImGui::Text("Hi.child~~");
     //ImGui::EndChildFrame();

     auto sksksk = 0;
    });
#endif
   //ImGui::EndChildFrame();
   //ImGui::End();
#if 0
   skin::tfSkinNodes parser_node_s;
   if (m_pHost)
    parser_node_s = dynamic_cast<const DearImGui*>(m_pHost)->SkinParserNodes();
   if (!(parser_node_s.size() > 1))
    break;
   for (auto it = parser_node_s.begin() + 1; it != parser_node_s.end(); ++it) {
    auto pNode = *it;
    if (!pNode)
     continue;

    const ControlType control_type = pNode->GetType();
    switch (control_type) {
    case ControlType::VerticalLayout: {
     auto parent = pNode->Parent();
     auto sizeMainWnd = ImGui::GetWindowSize();
     //ImGui::SetNextWindowSize(subWindowSize);
     //ImGui::SetNextWindowPos(subWindowPos);

     ImGui::PushStyleColor(ImGuiCol_WindowBg, \
      ImVec4{ pNode->BkColorArgb().r, pNode->BkColorArgb().g, pNode->BkColorArgb().b, pNode->BkColorArgb().a });
     ImGui::PushStyleColor(ImGuiCol_Border, \
      ImVec4{ pNode->BorderColorArgb().r, pNode->BorderColorArgb().g, pNode->BorderColorArgb().b, pNode->BorderColorArgb().a });

     ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_Alpha, pNode->Alpha());
     ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, pNode->Rounding());
     ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize, pNode->BorderSize());

     ImGui::Begin(pNode->TextA()[0] == '\0' ? " " : pNode->TextA(), &show_another_window/*,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground*/
      /*, ImGuiWindowFlags_NoTitleBar*/
     );
     if (pNode->Floating())
      ImGui::SetWindowPos(ImVec2(pNode->Pos().x, pNode->Pos().y));
     else
      ImGui::SetWindowPos(ImVec2(0, 0));
     ImGui::SetWindowSize(ImVec2(200, 300));

     ImGui::Text("Hi.~~");
     ImGui::End();
     ImGui::PopStyleColor(2);
     ImGui::PopStyleVar(3);
    }break;
    default:
     break;
    }

    auto sksksk = 0;
   }
#endif

   result = true;
  } while (0);
  return result;
 }

}///namespace local
