#include "stdafx.h"

namespace local {
 Skin::Skin() {
  Init();
 }
 Skin::~Skin() {
  UnInit();
 }
 void Skin::Init() {
  m_pResources = dynamic_cast<IResources*>(new Resources());
 }
 void Skin::UnInit() {
  m_IsOpen.store(false);

  if (!m_Controls.empty() && m_Controls[0]->Bind())
   dynamic_cast<IControlUI*>(m_Controls[0]->Bind())->OnRenderEnd();

  for (auto rit = m_Controls.rbegin(); rit != m_Controls.rend(); ++rit) {
   if ((*rit)->Bind())
    (*rit)->Bind()->Release();
   (*rit)->Release();
  }

  m_Controls.clear();

  SK_RELEASE_PTR(m_pResources);
 }
 void Skin::Release() const {
  delete this;
 }
 void Skin::SetUIModule(ISkinUI* pUI) {
  std::lock_guard<std::mutex> lock{*m_Mutex};
  m_pUI = pUI;
 }
 bool Skin::SkinConfigure(const char* skin_file_pathanme) {
  bool result = false;
  std::lock_guard<std::mutex> lock{*m_Mutex};
  do {
   if (!skin_file_pathanme)
    break;
   m_SkinFileName = shared::Win::PathFixedA(skin_file_pathanme);
   m_SkinDirectory = shared::Win::GetPathByPathnameA(m_SkinFileName);
   if (!shared::Win::AccessA(m_SkinDirectory) || !shared::Win::AccessA(m_SkinFileName))
    break;
   m_SkinFileBuffer = shared::Win::File::ReadCXX(m_SkinFileName);
   if (m_SkinFileBuffer.empty())
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Skin::Perform() {
  std::unique_lock<std::mutex> lock{*m_Mutex, std::defer_lock};
  lock.lock();
  do {
   if (m_IsOpen.load())
    break;
   if (!m_pUI)
    break;
   if (m_SkinFileBuffer.empty())
    break;

   TypeIdentify id = 0;
   if (!Parser::parser_xml(m_SkinFileBuffer,
    [this, &id](const NodeType& node_type, const std::uintptr_t& parent_node_key, const std::uintptr_t& node_key, const std::map<AttributeType, String>& attribute_s) {
     Control* control = nullptr;
     switch (node_type) {
      ////////////////////////////////////////////////////////////////////////////////////////
     case NodeType::Window: {
      control = dynamic_cast<Control*>(new Window());
      *control << attribute_s;
      m_Controls.emplace_back(control);
     }break;
     case NodeType::Frame: {
      control = dynamic_cast<Control*>(new Frame());
      *control << attribute_s;
      m_Controls.emplace_back(control);
     }break;
     case NodeType::VerticalLayout: {
      control = dynamic_cast<Control*>(new VerticalLayout());
      *control << attribute_s;
      m_Controls.emplace_back(control);
     }break;
     case NodeType::HorizontalLayout: {
      control = dynamic_cast<Control*>(new HorizontalLayout());
      *control << attribute_s;
      m_Controls.emplace_back(control);
     }break;
     case NodeType::Button: {
      control = dynamic_cast<Control*>(new Button());
      *control << attribute_s;
      m_Controls.emplace_back(control);
     }break;
      ////////////////////////////////////////////////////////////////////////////////////////
     case NodeType::Resources: {

     }break;
     case NodeType::Font: {
      control = dynamic_cast<Control*>(new Font(m_SkinDirectory));
      *control << attribute_s;
      m_pResources->Push(control);
     }break;
     case NodeType::Image: {
      control = dynamic_cast<Control*>(new Image(m_SkinDirectory));
      *control << attribute_s;
      m_pResources->Push(control);
     }break;
     case NodeType::UserData: {
      control = dynamic_cast<Control*>(new UserData(m_SkinDirectory));
      *control << attribute_s;
      m_pResources->Push(control);
     }break;
     default:
      break;
     }

     if (control) {
      control->Identify(id++);
      control->ParentNodeKey(parent_node_key);
      control->NodeKey(node_key);
     }
    }))
    break;

    // 1. Configure the parent class for controls.
    // 2. Configure child controls.
    // 3. Synchronization identify.
    // 4. Synchronization attributes.
    // 5. Configure ref resource cache.
    for (auto pControlSK : m_Controls) {
     do {
      auto pControlSKAttributes = pControlSK->GetAttributes();
      if (!pControlSKAttributes)
       break;
      pControlSKAttributes->Iterator(
       [this](const AttributeType& type, IAttribute* att, bool& itbreak) {
        switch (type) {
        case AttributeType::floating: {
         if (!att->GetBool())
          break;
        }break;
        case AttributeType::bkimage:
         [[fallthrough]];
        case AttributeType::logo: {
         auto res = m_pResources->Search(att->GetIdentify());
         if (!res)
          break;
         att->ResourceRef(res);
        }break;
        default:
         break;
        }

       });
     } while (0);

     IControlUI* pControlUI = nullptr;
     do {// Bind conctrl
      pControlUI = m_pUI->CreateControl(pControlSK->GetControlType());
      if (!pControlUI)
       break;
      pControlUI->Identify(pControlSK->Identify());
      pControlUI->Bind(pControlSK);
      pControlSK->Bind(pControlUI);
      pControlUI->SetAttributes(pControlSK->GetAttributes());
     } while (0);

     if (pControlSK->ParentNodeKey() <= 0)
      continue;

     // Find the parent node for the current control.
     for (auto& it : m_Controls) {
      if (pControlSK->ParentNodeKey() == it->NodeKey()) {
       // Set 'control' as a child of 'it' and 'it' as the parent of 'control'.
       it->PushChild(dynamic_cast<IControl*>(pControlSK));
       pControlSK->Parent(dynamic_cast<IControl*>(it));
       if (pControlUI)
        pControlUI->Parent(it->Bind());
       break;
      }
     }



    }
    m_IsOpen.store(true);
  } while (0);
  lock.unlock();
  //!@ Frist render.
  //Render();
  dynamic_cast<IControlUI*>(m_Controls[0]->Bind())->OnRenderBegin();
  dynamic_cast<IControlUI*>(m_Controls[0]->Bind())->RenderingFlags(true);

  return m_IsOpen.load();
 }
 bool Skin::Ready() const {
  std::lock_guard<std::mutex>(*m_Mutex);
  return m_IsOpen.load() && m_Ready.load();
 }

 static void TraverseRenderIterator(Control* pControl, const std::function<void(IControlSK*)>& begin_cb, const std::function<void(IControlSK*)>& end_cb) {
  do {
   if (!pControl)
    break;
   //!@ The main window does not render independently.
   /*if (pControl->Parent())*/
   begin_cb(pControl);
   auto childs = pControl->Childs();
   if (childs->Empty())
    break;
   bool end = false;
   childs->Iterator(
    [&](IControl* ctrl, bool& itbreak) {
     TraverseRenderIterator(dynamic_cast<Control*>(ctrl), begin_cb, end_cb);
     end_cb(dynamic_cast<IControlSK*>(ctrl));
     end = true;
    });
   if (!end)
    end_cb(dynamic_cast<IControlSK*>(pControl));
  } while (0);
 }

 static void TraverseLayoutIterator(Control* pControl, const std::function<void(Control*, const IControls*)>& layout_cb) {
  do {
   if (!pControl)
    break;
   if (!layout_cb)
    break;
   auto child_s = pControl->Childs();
   if (child_s->Empty())
    break;
   layout_cb(pControl, child_s);
   child_s->Iterator(
    [&](IControl* ctrl, bool& itbreak) {
     TraverseLayoutIterator(dynamic_cast<Control*>(ctrl), layout_cb);
    });
  } while (0);
 }
 void Skin::Layout() {
  std::unique_lock<std::mutex> lock(*m_Mutex, std::defer_lock);
  lock.lock();
  do {
   if (!m_pUI)
    break;
   if (!m_Ready.load())
    break;
   if (m_Controls.empty())
    break;

   //!@ 配置控件布局
   TraverseLayoutIterator(m_Controls[0],
    [this](Control* parent, const IControls* child_s) {
     const auto ParentSK = dynamic_cast<IControlSK*>(parent);
     const auto ParentUI = dynamic_cast<IControlUI*>(parent->Bind());
     const auto ParentLayout = ParentUI->GetLayoutData();
     Vec2 ParentSize;
     if (parent->GetControlType() == ControlType::Window)
      ParentSize = ParentUI->GetSize();
     else
      ParentSize = ParentLayout->empty() ? ParentUI->GetSize() : ParentLayout->GetSize();
     const auto LayoutCount = child_s->LayoutCount();
     long LayoutCountRemain = LayoutCount;
     //!@ 横向布局剩余空间
     unsigned long HorizontalRemainCX = ParentSize.GetCX();
     //!@ 竖向布局剩余空间
     unsigned long VerticalLayoutRemainCY = ParentSize.GetCY();

     if (parent->GetAttributes()->GetAttribute(AttributeType::name) && \
      parent->GetAttributes()->GetAttribute(AttributeType::name)->GetString()->compare("v1")) {


      auto sk = 0;
     }

     child_s->LayoutIterator(
      [&](IControl* child, bool& itbreak) {
       auto pChildSK = dynamic_cast<IControlSK*>(child);
       auto pChildUI = dynamic_cast<IControlUI*>(child->Bind());
       const Vec4 LayoutConifg(
        pChildSK->GetPosition().GetX(),
        pChildSK->GetPosition().GetY(),
        pChildSK->GetSize().GetCX(),
        pChildSK->GetSize().GetCY()
       );

       Vec4 LayoutFinal;
       LayoutFinal.SetPosition(pChildSK->GetPosition());
       LayoutFinal.SetSize(pChildSK->GetSize());

       switch (parent->GetLayoutType()) {
       case LayoutType::Vertical: {
        if (LayoutCountRemain <= 0)
         break;
        LayoutFinal.SetX(parent->GetLayoutData()->GetPosition().x);
        LayoutFinal.SetY(ParentSize.GetCY() - VerticalLayoutRemainCY);
        LayoutFinal.SetCX(LayoutConifg.GetCX() > 0 ? LayoutConifg.GetCX() : ParentSize.GetCX());
        if (LayoutFinal.GetCX() > ParentSize.GetCX())
         LayoutFinal.SetCX(ParentSize.GetCX());

        LayoutFinal.SetCY(LayoutConifg.GetCY() > 0 ? LayoutConifg.GetCY() : VerticalLayoutRemainCY / LayoutCountRemain);
        if (LayoutFinal.GetCY() > ParentSize.GetCY())
         LayoutFinal.SetCY(ParentSize.GetCY());

        VerticalLayoutRemainCY -= LayoutFinal.GetCY();
        --LayoutCountRemain;
       }break;
       case LayoutType::Horizontal: {
        if (LayoutCountRemain <= 0)
         break;
        LayoutFinal.SetX(ParentSize.GetCX() - HorizontalRemainCX);
        LayoutFinal.SetY(parent->GetLayoutData()->GetPosition().y);
        LayoutFinal.SetCX(LayoutConifg.GetCX() > 0 ? LayoutConifg.GetCX() : HorizontalRemainCX / LayoutCountRemain);
        if (LayoutFinal.GetCX() > ParentSize.GetCX())
         LayoutFinal.SetCX(ParentSize.GetCX());

        LayoutFinal.SetCY(LayoutConifg.GetCY() > 0 ? LayoutConifg.GetCY() : ParentSize.GetCY());
        if (LayoutFinal.GetCY() > ParentSize.GetCY())
         LayoutFinal.SetCY(ParentSize.GetCY());

        HorizontalRemainCX -= LayoutFinal.GetCX();
        --LayoutCountRemain;
       } break;
       case LayoutType::Floating:
        [[fallthrough]];
       default:
        break;
       }

       if (LayoutFinal.GetCX() > 0 && LayoutFinal.GetCY() > 0)
        pChildUI->SetLayoutData(LayoutFinal);

#if 0
       const IString* pName = nullptr;
       auto name = pChildSK->GetAttributes()->GetAttribute(AttributeType::name);
       if (name)
        pName = name->GetString();
       if (pName)
        std::cout << std::format("{} size is ({},{},{},{})", pName->str(), LayoutFinal.x, LayoutFinal.y, LayoutFinal.z, LayoutFinal.w) << std::endl;
#endif

       pChildUI->OnLayout();
      });
    });
  } while (0);
  lock.unlock();
 }
 void Skin::Render() {
  do {
   if (!m_pUI)
    break;
   if (m_Controls.empty())
    break;

   std::unique_lock<std::mutex> lock(*m_Mutex, std::defer_lock);
   lock.lock();

   TraverseRenderIterator(m_Controls[0],
    [this](IControlSK* pControlSK) {
     auto pControlUI = dynamic_cast<IControlUI*>(pControlSK->Bind());
     do {
      if (!pControlUI)
       break;
      if (pControlUI->RenderingFlags())
       break;
      pControlUI->OnRenderBegin();
      pControlUI->RenderingFlags(true);
     } while (0);
    },
    [this](IControlSK* pControlSK) {
     auto pControlUI = dynamic_cast<IControlUI*>(pControlSK->Bind());
     do {
      if (!pControlUI)
       break;
      if (!pControlUI->RenderingFlags())
       break;
      pControlUI->OnRenderEnd();
      pControlUI->RenderingFlags(false);
     } while (0);
    });
   if (!m_Ready.load())
    m_Ready.store(true);
   lock.unlock();

   Layout();

  } while (0);

 }







 HINSTANCE __gpHinstance = nullptr;
}///namespace local