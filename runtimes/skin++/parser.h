#if !defined(__0B21DEC5_0C02_4834_AC78_057CA045BCED__)
#define __0B21DEC5_0C02_4834_AC78_057CA045BCED__

namespace local {

 class Parser {
 public:
  Parser();
  ~Parser();
 public:
  static bool parser_xml(const std::string& xml_buffer, const std::function<void(const NodeType&, const std::uintptr_t&, const std::uintptr_t&, const std::map<AttributeType, String>& /*attribute_s*/)>&);
  static void parser_rgba(const TypeArgb&, std::uint8_t& a, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b);
  static void parser_rgba(const TypeArgb&, Vec4&);
  static bool parser_node_type(const String& node_name, NodeType&);
  static bool parser_node_attribute_type(const String& node_attribute_name, AttributeType&);
 };



}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 00:27:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0B21DEC5_0C02_4834_AC78_057CA045BCED__