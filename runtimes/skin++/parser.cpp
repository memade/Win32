#include "stdafx.h"

namespace local {

 Parser::Parser() {

 }

 Parser::~Parser() {

 }

 static void TraverseNode(rapidxml::xml_node<char>* node, \
  const std::function<void(rapidxml::xml_node<char>*)>& parser_n_cb) {
  if (parser_n_cb)
   parser_n_cb(node);
  //std::cout << node->name() << std::endl;
  //for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
  // std::cout << "Attribute: " << attr->name() << " = " << attr->value() << std::endl;
  //}
  auto node_name = node->name();
  for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {

   TraverseNode(child, parser_n_cb);
  }
 }
 void Parser::parser_rgba(const TypeArgbType& input_rgba, std::uint8_t& a, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b) {
  a = static_cast<std::uint8_t>((input_rgba & 0xFF000000) >> (4 * 6));
  r = static_cast<std::uint8_t>((input_rgba & 0xFF0000) >> (4 * 4));
  g = static_cast<std::uint8_t>((input_rgba & 0xFF00) >> (4 * 2));
  b = static_cast<std::uint8_t>((input_rgba & 0xFF) >> (4 * 0));
  if (a == 0)
   a = 0xFF;
 }
 bool Parser::parser_xml(const IResources* pIRes, 
  const std::function<INodeRender* (const IResources*, const ControlType&)>& node_create_cb, 
  const std::function<void(INodeRender*)>& create_success_cb) {
  bool result = false;
  if (!node_create_cb)
   return result;
  auto pRes = dynamic_cast<const Resources*>(pIRes);
  assert(pRes);
  auto pDoc = shared::rapidxml_parse(pRes->Cache());
  do {
   if (!pDoc)
    break;
   auto first_node = pDoc->first_node();
   do {
    if (!first_node)
     break;

    tfSkinNodes parser_node_s;

    TraverseNode(first_node,
     [&node_create_cb, &pIRes, &parser_node_s, &create_success_cb](auto node) {
      const char* node_name = node->name();
      ControlType node_type = ControlType::Unknown;
      if (!stricmp("Window", node_name)) {
       node_type = ControlType::Window;
      }
      if (!stricmp("Frame", node_name)) {
       node_type = ControlType::Frame;
      }
      else if (!stricmp("Control", node_name)) {
       node_type = ControlType::Control;
      }
      else if (!stricmp("Container", node_name)) {
       node_type = ControlType::Container;
      }
      else if (!stricmp("Label", node_name)) {
       node_type = ControlType::Label;
      }
      else if (!stricmp("Button", node_name)) {
       node_type = ControlType::Button;
      }
      else if (!stricmp("HorizontalLayout", node_name)) {
       node_type = ControlType::HorizontalLayout;
      }
      else if (!stricmp("VerticalLayout", node_name)) {
       node_type = ControlType::VerticalLayout;
      }
      else if (!stricmp("TabLayout", node_name)) {
       node_type = ControlType::TabLayout;
      }
      else if (!stricmp("Font", node_name)) {
       node_type = ControlType::Font;
      }


      do {
       if (node_type == ControlType::Unknown)
        break;
       auto pINode = node_create_cb(pIRes, node_type);
       auto pNode = dynamic_cast<NodeRender*>(pINode);
       if (!pINode || !pNode)
        break;
       pINode->NodeKey(reinterpret_cast<std::uintptr_t>(node));
       do {
        std::uintptr_t parent = reinterpret_cast<std::uintptr_t>(node->parent());
        if (parser_node_s.empty())
         break;
        for (auto& it : parser_node_s) {
         if (parent == it->NodeKey()) {
          pINode->Parent(it);
          it->Push(pINode);
          break;
         }
        }
       } while (0);

       parser_node_s.push_back(pINode);

       for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        *pNode << std::array<Strings, 2>{//!@ input utf8 data
         attribute->name() ?
          Strings(std::u8string(reinterpret_cast<const char8_t*>(attribute->name()), attribute->name_size())) : Strings(),
          attribute->value() ?
          Strings(std::u8string(reinterpret_cast<const char8_t*>(attribute->value()), attribute->value_size())) : Strings()};
       }
       if (create_success_cb)
        create_success_cb(pINode);
      } while (0);
     });

    first_node = first_node->next_sibling();
   } while (0);

   result = true;
  } while (0);
  SK_DELETE_PTR(pDoc);
  return result;
 }
}///namespace local