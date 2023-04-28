#include <win.hpp>
#include <zip++.h>
#include <cmdline.hpp>

int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3848);
#endif

 cmd::Cmdline cmdObj(::GetCommandLineW());

 size_t total_clean = 0;
 size_t total_scan = 0;
 //const std::vector<std::string> assign_file_format{".cdr",".jpg",".png",".bmp",".psd",""};
 do {
  auto assign_path = cmdObj.GetValue(L"assign_path");
  if (assign_path.empty())    
   return 0;
  if (!shared::Win::AccessW(assign_path))
   break;
  std::string assign_path_a = shared::IConv::WStringToMBytes(assign_path);
  tfEnumFolderNode files, dirs;
  shared::Win::EnumFoldersAndFiles(assign_path_a, dirs, files, "*.*", true);
  total_scan = files.size();

  std::map<size_t, std::vector<std::string>> work_map;
  for (const auto& node : files) {
   auto relative_pathname = node.first;
   auto file_name = node.second;
   auto file_pathname = shared::Win::PathFixedA(assign_path_a + "\\" + relative_pathname);
   size_t file_size = shared::Win::File::GetSizeA(file_pathname);
   if (file_size <= 0)
    continue;
   auto found_node = work_map.find(file_size);
   if (found_node == work_map.end()) {
    work_map.emplace(file_size, std::vector<std::string>{file_pathname});
   }
   else {
    found_node->second.emplace_back(file_pathname);
   }
  }

  for (auto it = work_map.begin(); it != work_map.end(); ++it) {
   if (it->second.size() > 1) {
    std::string first = it->second[0];
    //it->second.erase(it->second.begin());
    do {
     if (it->second.empty())
      break;
     bool handle = false;
     for (auto it_file = it->second.begin() + 1; it_file != it->second.end();) {

      std::string file_name_first;
      std::string file_format_first;
      shared::Win::GetFileNameAndFormat(first, file_name_first, file_format_first);

      std::string file_name;
      std::string file_format;
      shared::Win::GetFileNameAndFormat(*it_file, file_name, file_format);
      if (file_format.compare(file_format_first) != 0) {
       ++it_file;
       continue;
      }

      do {
       if (file_name_first.empty() || file_format_first.empty())
        break;
       auto found = file_name_first.find('(');
       if (found == std::string::npos)
        break;
       first.resize(first.size() - (file_name_first.size() - found + file_format.size()));
      } while (0);

      if (strncmp(it_file->c_str(), first.c_str(), __min(it_file->size() - file_format.size(), first.size() - file_format.size())) != 0) {
       ++it_file;
       continue;
      }
      ::SetFileAttributesA(it_file->c_str(), FILE_ATTRIBUTE_NORMAL);
      int remove_ret = ::remove(it_file->c_str());
      ++total_clean;
      std::cout << std::format("移除重复文件[{}] {}({})", *it_file, remove_ret == 0 ? "success" : "failed", remove_ret) << std::endl;
      it_file = it->second.erase(it_file);
      handle = true;
     }
     if (!handle) {
      it->second.erase(it->second.begin());
     }
     if (it->second.size() <= 1)
      break;
     first = it->second[0];
    } while (1);
   }
  }

 } while (0);

 std::cout << std::format("操作完成！枚举文件总数:[{}] 清理文件总数:[{}].", total_scan, total_clean) << std::endl;
 


#if 0
 std::string zip_test = R"(#asglkjasdgh;asdkglasd32151346236234asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563asdkglasd32151346236234664563664563!)";
 std::string gzip_ret, gzip_ret_un;
 shared::Zip::gzipCompress(zip_test, gzip_ret);
 shared::Zip::gzipUnCompress(gzip_ret, gzip_ret_un);

 std::string zip_ret, unzip_ret;
 shared::Zip::zipCompress(zip_test, zip_ret);
 shared::Zip::zipUnCompress(zip_ret, zip_test.size(), unzip_ret);
#endif




 shared::Win::MainProcess(
  [&](const std::string& input, bool& exit) {

   if (input == "q") {
    exit = true;
   }

  });
 return 0;
}