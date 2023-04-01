#include <win.hpp>
#include <wxskin.hpp>


int main(int argc, char** argv) {
#if defined(_DEBUG)
 ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //::_CrtSetBreakAlloc(3709);
#endif


 shared::Win::MainProcess(
  [](const std::string& input,bool& exit) {
  
  
  
  });
 return 0;
}