#if !defined(__DCEA3F15_2301_46E0_BA67_383E9CD345D8__)
#define __DCEA3F15_2301_46E0_BA67_383E9CD345D8__

namespace shared {
 /*
* 2023-04-29T13:39:25+0800
* 2023-04-29T13:39:25.724+0800
*/
 static std::string format_current_time(bool with_milliseconds) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::gmtime(&now_c), "%FT%T");
  if (with_milliseconds) {
   auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    now.time_since_epoch()
   ) % 1000;
   ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
  }
  ss << std::put_time(std::gmtime(&now_c), "%z");
  return ss.str();
 }
 /* 获取本地时区的偏移量
 * ISO 标准时间表示法中，字母 "Z" 表示协调世界时（UTC）时间。它表示时间与 UTC 时间相同，即没有时区偏移量
 * result: +0800 or Z
 *
 */
 static std::string GetLocalTimezoneOffset() {
  std::string result;
  std::time_t now = std::time(nullptr);
  std::tm local_tm = *std::localtime(&now);
  std::tm utc_tm = *std::gmtime(&now);
  // 本地时区的偏移量为UTC时间减去本地时间的秒数
  int local_offset_seconds =
   std::difftime(std::mktime(&local_tm), std::mktime(&utc_tm));
  int hours = local_offset_seconds / 3600;
  int minutes = std::abs(local_offset_seconds % 3600) / 60;
  // 生成偏移量的字符串表示
  if (hours == 0) {
   result = "Z";
  }
  else {
   result =
    ((hours >= 0) ? "+" : "-") +
    (hours < 10 ? "0" + std::to_string(std::abs(hours))
     : std::to_string(std::abs(hours))) +
    (minutes < 10 ? "0" + std::to_string(minutes)
     : std::to_string(minutes));
  }
  return result;
 }
 /**
 * ISO 标准时间表示法是指国际标准化组织（ISO）制定的一套时间表示方法，用于在全球范围内统一时间表示方式，避免由于时区和文化差异造成的混淆。
 * ISO 标准时间表示法的格式为：YYYY-MM-DDTHH:mm:ss.sssZ
 * @return ISO 标准时间表示法的字符串，例如 "2023-04-29T13:30:45.500+08:00"
 * 2023-04-29T14:22:37.189Z =>(UTC)
 * 2023-04-29T14:22:37.189+0800 =>(LOCAL)
 *
 */
 template<typename T = std::chrono::seconds>
  requires (std::is_same_v<T, std::chrono::milliseconds> || std::is_same_v<T, std::chrono::seconds>)
 static std::string TimestampToISO8601(std::uint64_t timestamp, const bool& local = true/*or UTC == false*/) {
  std::ostringstream os;
  auto ms = T(timestamp);
  auto tp = std::chrono::time_point<std::chrono::system_clock, T>(ms);
  auto time_t = std::chrono::system_clock::to_time_t(tp);
  if (timestamp > 9999999999) {
   os << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S.");
   os << std::setfill('0') << std::setw(3) << (timestamp % 1000);
  }
  else {
   os << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
  }
  local ? os << GetLocalTimezoneOffset() : os << "Z";
  return os.str();
 }


 template<typename T = std::chrono::seconds>
  requires (std::is_same_v<T, std::chrono::milliseconds> || std::is_same_v<T, std::chrono::seconds>)
 static std::time_t TimeStampUTC() {
  return std::chrono::duration_cast<T>(std::chrono::time_point_cast<T>(std::chrono::system_clock::now()).time_since_epoch()).count();
 }


}///namespace shared

/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 29 Apr 2023 08:55:14 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__DCEA3F15_2301_46E0_BA67_383E9CD345D8__