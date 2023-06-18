#if !defined(__5BA3DE8B_8069_4236_AA14_4D5205FC3039__)
#define __5BA3DE8B_8069_4236_AA14_4D5205FC3039__

#include <dllinterface.hpp>

namespace cef {

 class IConfig {
 public:

 };


 class ICef : public shared::InterfaceDll<ICef> , public shared::IUserInterface {
 public:
  virtual IConfig* ConfigGet() const = 0;
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual void Release() const = 0;
 };




}///namespace cef



/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 02 Jun 2023 11:47:32 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__5BA3DE8B_8069_4236_AA14_4D5205FC3039__