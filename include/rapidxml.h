#if !defined(INC_H___7D04D217_163A_4A9C_8FDA_DF9907B7BC28__HEAD__)
#define INC_H___7D04D217_163A_4A9C_8FDA_DF9907B7BC28__HEAD__

#include "../third_party/rapidxml/rapidxml.hpp"
#include "../third_party/rapidxml/rapidxml_iterators.hpp"
#include "../third_party/rapidxml/rapidxml_print.hpp"
#include "../third_party/rapidxml/rapidxml_utils.hpp"

namespace shared {

 using tf_apidxml_char_doc = rapidxml::xml_document<char>;
 using tf_rapidxml_wchar_doc = rapidxml::xml_document<wchar_t>;

 static tf_rapidxml_wchar_doc* rapidxml_parse(const std::wstring& input_data) {
  auto pDoc = new tf_rapidxml_wchar_doc();
  try {
   pDoc->parse<0>((wchar_t*)input_data.c_str());
  }
  catch (rapidxml::parse_error& err) {
   auto what = err.what();
   if (pDoc) {
    delete pDoc;
    pDoc = nullptr;
   }
  }
  return pDoc;
 }

 static tf_apidxml_char_doc* rapidxml_parse(const std::string& input_data) {
  auto pDoc = new tf_apidxml_char_doc();
  try {
   pDoc->parse<0>((char*)input_data.c_str());
  }
  catch (rapidxml::parse_error& err) {
   auto what = err.what();
   if (pDoc) {
    delete pDoc;
    pDoc = nullptr;
   }
  }
  return pDoc;
 }
 static std::string rapidxml_output(rapidxml::xml_node<char>* input_xml_node) {
  std::string result;
  do {
   if (!input_xml_node)
    break;
   rapidxml::print(std::back_inserter(result), *input_xml_node, 0);
  } while (0);
  return result;
 }

}///namespace shared


/// /*新生®（上海）**/
/// /*2022_02_21T03:51:21.4884715Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___7D04D217_163A_4A9C_8FDA_DF9907B7BC28__HEAD__
