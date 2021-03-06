#include <cstdint>

class Color {
 public:
  uint8_t r;
  uint8_t g;
  uint8_t b;

  Color();
  Color(uint8_t, uint8_t, uint8_t);
  static Color random();

  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color White;
  static const Color Black;
};
