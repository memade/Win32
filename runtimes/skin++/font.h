#if !defined(__30BD696D_3B24_4328_BAB5_36CE22CF28F8__)
#define __30BD696D_3B24_4328_BAB5_36CE22CF28F8__

namespace local {

#if 0
 class FontTmp final : public skin::IFont {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  FontTmp();
  virtual ~FontTmp();
 protected:
  const TypeIdentifyType& Identify() const override final;
  const bool& Bold() const override final;
  const ICache* GetCache() const override final;
  void* Handle() const override final;
  const IString* FilePathname() const override final;
 private:
  TypeIdentifyType identify_ = 0;
  Cache font_cache_;
  String file_pathname_;
  bool bold_ = false;
 };
#endif


}///namespace local

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 02 Jul 2023 12:44:38 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__30BD696D_3B24_4328_BAB5_36CE22CF28F8__