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
  auto node_name = node->name();
  for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling())
   TraverseNode(child, parser_n_cb);
 }
 bool Parser::parser_xml(const std::string& xml_buffer, const std::function<void(const NodeType&, const std::uintptr_t&, const std::uintptr_t&, const std::map<AttributeType, String>&)>& parser_cb) {
  bool result = false;
  auto pDoc = shared::rapidxml_parse(xml_buffer);
  do {
   if (!pDoc)
    break;

   auto root_node = pDoc->first_node();
   do {
    if (!root_node)
     break;
    TraverseNode(root_node,
     [&parser_cb](rapidxml::xml_node<char>* node) {

      std::uintptr_t parent_node_key = reinterpret_cast<std::uintptr_t>(node->parent());
      if (node->type() == rapidxml::node_type::node_document || node->parent()->name_size() <= 0)
       parent_node_key = 0;
      std::uintptr_t node_key = reinterpret_cast<std::uintptr_t>(node);

      NodeType node_type = NodeType::Unknown;
      String node_name(std::u8string(reinterpret_cast<const char8_t*>(node->name()), node->name_size()));
      do {
       if (!parser_node_type(node_name, node_type))
        break;
       std::map<AttributeType, String> attribute_s;
       for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (!attribute->name() || attribute->name_size() <= 0)
         continue;
        AttributeType attribute_type = AttributeType::unknown;
        String attribute_name(attribute->name() ? std::u8string(reinterpret_cast<const char8_t*>(attribute->name()), attribute->name_size()) : String());
        if (!parser_node_attribute_type(attribute_name, attribute_type))
         continue;
        String attribute_value(attribute->value() ? std::u8string(reinterpret_cast<const char8_t*>(attribute->value()), attribute->value_size()) : String());
        auto found = attribute_s.find(attribute_type);
        if (found != attribute_s.end())
         attribute_s.erase(found);
        attribute_s.emplace(attribute_type, attribute_value);
       }
       if (parser_cb)
        parser_cb(node_type, parent_node_key, node_key, attribute_s);
      } while (0);
     });
    root_node = root_node->next_sibling();
   } while (0);

   result = true;
  } while (0);
  SK_DELETE_PTR(pDoc);
  return result;
 }
 bool Parser::parser_node_attribute_type(const String& node_attribute_name, AttributeType& out_attribute_type) {
  bool result = false;
  out_attribute_type = AttributeType::unknown;
  const bool node_name_not_case_sensitive = true;
  do {
   if (node_attribute_name.compare("identify", node_name_not_case_sensitive) ||
    node_attribute_name.compare("id", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::identify;
   else if (node_attribute_name.compare("position", node_name_not_case_sensitive) ||
    node_attribute_name.compare("pos", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::position;
   else if (node_attribute_name.compare("sizebox", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::sizebox;
   else if (node_attribute_name.compare("caption", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::caption;
   else if (node_attribute_name.compare("size", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::size;
   else if (node_attribute_name.compare("width", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::width;
   else if (node_attribute_name.compare("height", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::height;
   else if (node_attribute_name.compare("borderround", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::borderround;
   else if (node_attribute_name.compare("roundcorner", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::roundcorner;
   else if (node_attribute_name.compare("minsize", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::minsize;
   else if (node_attribute_name.compare("maxsize", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::maxsize;
   else if (node_attribute_name.compare("bkcolor", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::bkcolor;
   else if (node_attribute_name.compare("bordercolor", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::bordercolor;
   else if (node_attribute_name.compare("bordersize", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::bordersize;
   else if (node_attribute_name.compare("visible", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::visible;
   else if (node_attribute_name.compare("rounding", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::rounding;
   else if (node_attribute_name.compare("alpha", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::alpha;
   else if (node_attribute_name.compare("floating", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::floating;
   else if (node_attribute_name.compare("font", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::font;
   else if (node_attribute_name.compare("class_name", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::class_name;
   else if (node_attribute_name.compare("title_name", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::title_name;
   else if (node_attribute_name.compare("bkimage", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::bkimage;
   else if (node_attribute_name.compare("logo", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::logo;
   else if (node_attribute_name.compare("name", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::name;
   else if (node_attribute_name.compare("text", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::text;
   else if (node_attribute_name.compare("layout", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::layout;
   else if (node_attribute_name.compare("maximize", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::maximize;
   else if (node_attribute_name.compare("file", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::file;
   else if (node_attribute_name.compare("bold", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::bold;
   else if (node_attribute_name.compare("center", node_name_not_case_sensitive))
    out_attribute_type = AttributeType::center;
   else
    break;
   result = true;
  } while (0);
  return result;
 }
 bool Parser::parser_node_type(const String& node_name, NodeType& out_node_type) {
  bool result = false;
  out_node_type = NodeType::Unknown;
  const bool node_name_not_case_sensitive = true;
  do {
   if (node_name.compare("Window", node_name_not_case_sensitive))
    out_node_type = NodeType::Window;
   else if (node_name.compare("Resources", node_name_not_case_sensitive))
    out_node_type = NodeType::Resources;
   else if (node_name.compare("Font", node_name_not_case_sensitive))
    out_node_type = NodeType::Font;
   else if (node_name.compare("Image", node_name_not_case_sensitive))
    out_node_type = NodeType::Image;
   else if (node_name.compare("UserData", node_name_not_case_sensitive))
    out_node_type = NodeType::UserData;
   else if (node_name.compare("Frame", node_name_not_case_sensitive))
    out_node_type = NodeType::Frame;
   else if (node_name.compare("HorizontalLayout", node_name_not_case_sensitive))
    out_node_type = NodeType::HorizontalLayout;
   else if (node_name.compare("VerticalLayout", node_name_not_case_sensitive))
    out_node_type = NodeType::VerticalLayout;
   else if (node_name.compare("Button", node_name_not_case_sensitive))
    out_node_type = NodeType::Button;
   else
    break;
   result = true;
  } while (0);
  return result;
 }

 void Parser::parser_rgba(const TypeArgb& input_rgba, Vec4& outVec) {
  std::uint8_t a, r, g, b;
  parser_rgba(input_rgba, a, r, g, b);
  outVec.x = a / 255.0f;
  outVec.y = r / 255.0f;
  outVec.z = g / 255.0f;
  outVec.w = b / 255.0f;
 }

 void Parser::parser_rgba(const TypeArgb& input_rgba, std::uint8_t& a, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b) {
  a = static_cast<std::uint8_t>((input_rgba & 0xFF000000) >> (4 * 6));
  r = static_cast<std::uint8_t>((input_rgba & 0xFF0000) >> (4 * 4));
  g = static_cast<std::uint8_t>((input_rgba & 0xFF00) >> (4 * 2));
  b = static_cast<std::uint8_t>((input_rgba & 0xFF) >> (4 * 0));
  if (a == 0)
   a = 0x80;
 }

}///namespace local