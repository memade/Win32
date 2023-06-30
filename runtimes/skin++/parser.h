#if !defined(__0B21DEC5_0C02_4834_AC78_057CA045BCED__)
#define __0B21DEC5_0C02_4834_AC78_057CA045BCED__

namespace local {

 class Parser {
  using tf_parser_create_skin_control_cb = std::function<IControl* ()>;
  using tf_parser_node_control_cb = std::function<bool(IControl*)>;
 public:
  Parser();
  ~Parser();
 public:
  static bool parser_xml(const IResources*, 
   const std::function<INodeRender*(const IResources*, 
    const ControlType&)>&,const std::function<void(INodeRender*)>& create_success_cb = nullptr);
  static void parser_rgba(const TypeArgbType&, std::uint8_t& a, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b);
 };



}///namespace local


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 21 Jun 2023 00:27:36 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0B21DEC5_0C02_4834_AC78_057CA045BCED__