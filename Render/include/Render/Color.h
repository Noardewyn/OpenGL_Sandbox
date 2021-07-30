#ifndef __COLOR_H__
#define __COLOR_H__

namespace Renderer {

struct Color {
public:
  float r, g, b, a;
  
  Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}

  Color(float r, float g, float b, float a = 1.0f) 
  : r(r), g(g), b(b), a(a)
  {}

  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Black;
  static const Color White;
  static const Color Yellow;
  static const Color Magenta;
};

} // namespace Renderer

#endif // __COLOR_H__
