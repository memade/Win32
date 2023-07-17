#if !defined(__703A2F1B_F151_4E16_AFA6_9C14E0ECFBEA__)
#define __703A2F1B_F151_4E16_AFA6_9C14E0ECFBEA__

#include "dllinterface.hpp"
#include "cmdline.hpp"

namespace process {
 using TypeProcess = unsigned long;


 enum class ProcessType : TypeProcess {
  MASTER = 0x00000000,
  UTILITY = 0x01000000,

  BEGIN = MASTER,
  END = UTILITY,
  MIN = MASTER,
  MAX = 0xFF000000,

  DEFAULT = MASTER,
 };

 class IProcess {
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual void Release() const = 0;
  virtual HANDLE Handle() const = 0;
  virtual const ProcessType& Type() const = 0;
 };

 class ILaunch : public shared::InterfaceDll<ILaunch> {
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual void Release() const = 0;
 };


}///namespace process


/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 12 Jul 2023 00:45:49 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__703A2F1B_F151_4E16_AFA6_9C14E0ECFBEA__
