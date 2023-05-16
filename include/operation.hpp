#if !defined(__7A4EE285_6F4A_4327_AF91_365E173D0F8B__)
#define __7A4EE285_6F4A_4327_AF91_365E173D0F8B__

namespace shared {
 static const int SHIFT_DIRECTION_BASE = 4;
 // This is an enumeration class called ShiftDirection.
 // It has two values: LEFT and RIGHT.
 enum class ShiftDirection {
  LEFT,  // Represents a left shift.
  RIGHT  // Represents a right shift.
 };

 // This is a template function that takes a type T as its input.
// It requires that T is an integral type (integers and bool).
 template <typename T>
  requires std::integral<T>

 // This function takes three arguments:
 // 1. num: a constant reference to an object of type T.
 // 2. n: a constant reference to an unsigned 16-bit integer (std::uint16_t).
 // 3. dir: an enum value of type ShiftDirection (LEFT or RIGHT).

 // The function returns a value of type T.
 static T shift_hex_by_nibbles(const T& num, const std::uint16_t& n, const ShiftDirection& dir) {
  // Initialize a variable called "result" of type T with a value of zero.
  T result = 0;
  // Use a switch statement to handle the different cases for the ShiftDirection enum.
  switch (dir) {
   // If the ShiftDirection is LEFT, shift the bits of the input number to the left by n*4.
  case ShiftDirection::LEFT:
   result = num << (n * SHIFT_DIRECTION_BASE);
   break;
   // If the ShiftDirection is RIGHT, shift the bits of the input number to the right by n*4.
  case ShiftDirection::RIGHT:
   result = num >> (n * SHIFT_DIRECTION_BASE);
   break;
   // If neither LEFT nor RIGHT, do nothing.
  default:
   break;
  }
  // Return the result of the shift operation.
  return result;
 }




}///namespace shared


/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 29 Apr 2023 08:08:28 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__7A4EE285_6F4A_4327_AF91_365E173D0F8B__