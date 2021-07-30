#ifndef __VERTEX_BUFFER_LAYOUT_H__
#define __VERTEX_BUFFER_LAYOUT_H__

#include <iostream>
#include <cassert>
#include <vector>
#include <GL/glew.h>

namespace Renderer {

struct VertexBufferElement {
  uint32_t count;
  uint32_t type;
  unsigned char normalized;

  static uint32_t getTypeSize(uint32_t type) {
    switch (type) {
      case GL_FLOAT:         return 4;
      case GL_UNSIGNED_INT:  return 4;
      case GL_UNSIGNED_BYTE: return 1;
    }
    assert(false);

    return 0;
  }
};

class VertexBufferLayout {
public:

  template<typename T>
  void push(uint32_t count) {
    static_assert(false);
  }

  template<>
  void push<float>(uint32_t count) {
    _elements.push_back({count, GL_FLOAT, GL_FALSE});
    _stride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
  }

  template<>
  void push<unsigned int>(uint32_t count) {
    _elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
    _stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
  }

  template<>
  void push<unsigned char>(uint32_t count) {
    _elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
    _stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferElement> getElements() const { return _elements; }
  inline uint32_t getStride() const { return _stride; }

private:
  std::vector<VertexBufferElement> _elements;
  uint32_t _stride = 0;
};

} // namespace Renderer

#endif // __VERTEX_BUFFER_LAYOUT_H__
