#include "stdafx.h"

namespace local {
 Skin::Skin() {
  Init();
 }
 Skin::~Skin() {
  UnInit();
 }
 void Skin::Init() {

 }
 void Skin::UnInit() {
  m_IsOpen.store(false);
  for (auto& t : m_Threads)
   t.join();
  m_Threads.clear();
  for (const auto& node : m_Nodes) {
   OnNodeDestroy(node);
   node->Release();
  }
  m_Nodes.clear();
  for (const auto& node : m_ResNodes)
   node.second->Release();
  m_ResNodes.clear();
 }
 void Skin::Release() const {
  delete this;
 }
 void Skin::From(ISkinUI* pUI) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pUI = pUI;
 }
 bool Skin::From(const char* skin_directory) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!skin_directory)
    break;
   m_SkinDirectory = shared::Win::PathFixedA(skin_directory);
   if (!shared::Win::AccessA(m_SkinDirectory))
    break;
   tfEnumFolderNode dirs, files;
   //!@ 枚举skin xml 配置文件
   shared::Win::EnumFoldersAndFiles(m_SkinDirectory, dirs, files, "*.*", true,
    [&](const std::string& f_path, const std::string& f_name, const _finddata_t& find_data) {
     std::string name, format;
     shared::Win::GetFileNameAndFormat(f_name, name, format);
     IResources* pRes = nullptr;
     if (StrStrIA(".xml", format.c_str())) {
      pRes = dynamic_cast<IResources*>(new ResourcesConfigXml());
     }
     if (StrStrIA(".json", format.c_str())) {
      pRes = dynamic_cast<IResources*>(new ResourcesConfigJson());
     }
     else if (StrStrIA(".ico", format.c_str())) {
      pRes = dynamic_cast<IResources*>(new ResourcesImageIcon());
     }
     else if (StrStrIA(".ttf", format.c_str())) {
      pRes = dynamic_cast<IResources*>(new ResourcesFontTTF());
     }
     else if (StrStrIA(".ttc", format.c_str())) {
      pRes = dynamic_cast<IResources*>(new ResourcesFontTTC);
     }
     if (pRes) {
      pRes->Pathname(shared::Win::PathFixedA(m_SkinDirectory + "\\" + f_path).c_str());
      pRes->Path(shared::Win::GetPathByPathnameA(pRes->Pathname()).c_str());
      pRes->Load();
      m_ResNodes.emplace(pRes->Pathname(), pRes);
     }
    });
   result = true;
  } while (0);
  return result;
 }
 void Skin::OnNodeCreate(INodeRender* pINode) const {
  if (m_NodeCreateCb)
   m_NodeCreateCb(pINode);
 }
 void Skin::OnNodeDestroy(INodeRender* pINode) const {
  if (m_NodeDestroyCb)
   m_NodeDestroyCb(pINode);
 }
 void Skin::RegisterNodeCreateCb(const tfNodeCreateCb& create_cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_NodeCreateCb = create_cb;
 }
 void Skin::RegisterNodeDestroyCb(const tfNodeDestroyCb& destroy_cb) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_NodeDestroyCb = destroy_cb;
 }
 void Skin::ParserArgb(const TypeArgbType& input_argb, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  Parser::parser_rgba(input_argb, r, g, b, a);
 }
 static void TraverseNodeIterator(INodeRender* pINode, const tfNodeBeginCb& begin_cb, const tfNodeEndCb& end_cb) {
  do {
   auto pNode = dynamic_cast<const NodeRender*>(pINode);
   if (!pNode)
    break;
   begin_cb(pINode);
   auto nodes = pNode->Childs();
   if (nodes.empty())
    break;
   bool end = false;
   for (auto& node : nodes) {
    TraverseNodeIterator(node, begin_cb, end_cb);
    end_cb(node);
    end = true;
   }
   if (!end)
    end_cb(pINode);
   auto sksksk = 0;
  } while (0);
 }
 INodeRender* Skin::MainNode() const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  return m_pMainNode;
 }
 void Skin::FontNodeIterator(const tfNodeCb& node_cb) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  for (auto& node : m_Nodes) {
   if (node->GetType() != ControlType::Font)
    continue;
   node_cb(node);
  }
 }
 void Skin::NodeIterator(const tfNodeBeginCb& begin_cb, const tfNodeDestroyCb& end_cb) const {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  if (m_pMainNode) {
   TraverseNodeIterator(m_pMainNode, begin_cb, end_cb);
   end_cb(m_pMainNode);
  }
 }
 bool Skin::Perform() {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (m_IsOpen.load())
    break;
   if (!m_pUI)
    break;

   for (auto& node : m_ResNodes) {
    switch (node.second->Type()) {
    case ResourcesType::CONFIG_XML: {
     if (!node.second->Load())
      break;
     TypeIdentifyType identify = 0;
     if (!Parser::parser_xml(node.second,
      [this, &identify](const IResources* pRes, const ControlType& control_type) {
       INodeRender* result = nullptr;
       switch (control_type) {
       case ControlType::Window:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Window(identify++, pRes)));
        break;
       case ControlType::Frame:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Frame(identify++, pRes)));
        break;
       case ControlType::Control:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Control(identify++, pRes)));
        break;
       case ControlType::Container:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Container(identify++, pRes)));
        break;
       case ControlType::Label:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Label(identify++, pRes)));
        break;
       case ControlType::Button:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Button(identify++, pRes)));
        break;
       case ControlType::HorizontalLayout:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new HorizontalLayout(identify++, pRes)));
        break;
       case ControlType::VerticalLayout:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new VerticalLayout(identify++, pRes)));
        break;
       case ControlType::TabLayout:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new TabLayout(identify++, pRes)));
        break;
       case ControlType::Font:
        result = m_Nodes.emplace_back(dynamic_cast<INodeRender*>(new Font(identify++, pRes)));
        break;
       default:
        break;
       }

       return result;
      }, [this](INodeRender* pNodeCreateSuccess) {
       pNodeCreateSuccess->DefaultConfigCover();
       OnNodeCreate(pNodeCreateSuccess);
      }))
      break;
      m_pMainNode = m_Nodes[0];
    }break;
    case ResourcesType::IMAGE_ICON_: {

     auto sk = 0;
    }break;
    default:
     break;
    }
   }


   m_IsOpen.store(true);
   m_Threads.emplace_back([this]() {Render(); });
  } while (0);
  return m_IsOpen.load();
 }

 void Skin::Render() {
  do {
   do {
    if (!m_pUI)
     break;
    m_pUI->Render();
   } while (0);
   if (!m_IsOpen.load())
    break;
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (1);
 }







 HINSTANCE __gpHinstance = nullptr;
}///namespace local