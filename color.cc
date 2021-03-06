#include "color.h"

#include <random>

// Constructor definition
Color::Color() : r(0), g(0), b(0) {}
Color::Color(uint8_t red, uint8_t green, uint8_t blue)
    : r(red), g(green), b(blue) {}

// Random method
Color Color::random() {
  return Color(rand() % 256, rand() % 256, rand() % 256);
}

// Static constants
const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
