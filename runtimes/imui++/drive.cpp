#include "stdafx.h"

namespace local {
 static bool show_another_window = false;

 IDrive::IDrive(const IDearImGui* host, const Control* ctrl)
  : m_pHost(host)
  , m_pControl(ctrl) {
  m_Size.SetCX(1024);
  m_Size.SetCY(768);
  m_hCreate = ::CreateEventW(NULL, TRUE, FALSE, NULL);
#if IMGUI_DIRECTX_ENABLE
  m_WndClassEx.cbSize = sizeof(m_WndClassEx);
  m_WndClassEx.style = CS_CLASSDC;
  m_WndClassEx.lpfnWndProc = nullptr;
  m_WndClassEx.cbClsExtra = 0L;
  m_WndClassEx.cbWndExtra = 0L;
  m_WndClassEx.hInstance = __gpHinstance;
  m_WndClassEx.hIcon = nullptr;
  m_WndClassEx.hCursor = NULL;
  m_WndClassEx.hbrBackground = NULL;
  m_WndClassEx.lpszMenuName = NULL;
  m_WndClassEx.lpszClassName = /*m_pNodeSK->ClassName() ? m_pNodeSK->ClassName()->wstr() : */LR"(9E827EBF7EC6)";
  m_WndClassEx.hIconSm = nullptr;

  m_WndCreateStructW.lpCreateParams = nullptr;
  m_WndCreateStructW.hInstance = m_WndClassEx.hInstance;
  m_WndCreateStructW.hMenu = nullptr;
  m_WndCreateStructW.hwndParent = nullptr;
  m_WndCreateStructW.cy = CW_USEDEFAULT;
  m_WndCreateStructW.cx = CW_USEDEFAULT;
  m_WndCreateStructW.y = CW_USEDEFAULT;
  m_WndCreateStructW.x = CW_USEDEFAULT;
  m_WndCreateStructW.style = WS_OVERLAPPEDWINDOW;
  m_WndCreateStructW.lpszName = nullptr;
  m_WndCreateStructW.lpszClass = m_WndClassEx.lpszClassName;
  m_WndCreateStructW.dwExStyle = 0;
#endif///IMGUI_DIRECTX_ENABLE
 }
 IDrive::~IDrive() {
  SK_RELEASE_EVENT(m_hCreate);
 }
 void IDrive::CreateNotify() const {
  if (m_hCreate) {
   ::SetEvent(m_hCreate);
  }
 }
 void IDrive::CreateWait() const {
  if (m_hCreate)
   ::WaitForSingleObject(m_hCreate, INFINITE);
 }
 bool IDrive::Start() {
  do {
   if (m_IsOpen.load())
    break;
   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Process(); });
   CreateWait();
  } while (0);
  return m_IsOpen.load();
 }

 void IDrive::Stop() {
  do {
   if (!m_IsOpen.load())
    break;
   CreateNotify();
   m_IsOpen.store(false);
   for (auto& t : m_Threads)
    t.join();
   m_Threads.clear();
  } while (0);
 }
 const HWND& IDrive::Hwnd() const {
  return m_hWnd;
 }
 void IDrive::OnLoadFonts() {
  do {
#if 0
   if (!Host())
    break;
   if (!Host()->SkinGet())
    break;
   Host()->SkinGet()->FontNodeIterator(
    [](skin::INodeSK* pFontNode) {
     do {
      if (pFontNode->Mark())
       break;
      pFontNode->Mark(true);
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
#endif
  } while (0);
 }
 void IDrive::SetSize(const Vec2& size) {
  if (size.x != m_Size.x || size.y != m_Size.y)
   m_Size = size;
 }
 void IDrive::OnLayout() {
  do {
   //if (!m_pHost)
   // break;
   //if (!m_pHost->SkinGet())
   // break;
   //m_pHost->SkinGet()->Layout();
  } while (0);
 }
 bool IDrive::OnRender() {
  bool result = false;
  do {
   if (!m_pHost)
    break;
   if (!m_pHost->SkinGet())
    break;
   m_pHost->SkinGet()->Render();
   result = true;
  } while (0);
  return result;


#if 0
  ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
  ImGui::Text("Hello from another window!");
  if (ImGui::Button("Close Me"))
   show_another_window = false;
  ImGui::End();
#endif

#if 0
  Host()->SkinGet()->NodeIterator(
   [&](skin::INodeRender* pINode) {//!@ begin
    do {
     auto ctrl = reinterpret_cast<INodeRender*>(pINode->Route());
     if (!ctrl)
      break;
     ctrl->OnRenderBegin();
    } while (0);
   },
   [&](skin::INodeRender* pINode) {//!@ end
    do {
     auto ctrl = reinterpret_cast<INodeRender*>(pINode->Route());
     if (!ctrl)
      break;
     ctrl->OnRenderEnd();
    } while (0);
 });
#endif
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

  result = true;
 } while (0);
#endif

 return result;
 }

}///namespace local
