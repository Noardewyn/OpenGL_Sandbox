#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <iostream>

namespace Renderer {

class VertexBuffer {
public:
  VertexBuffer(const void* data, uint32_t size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;

  inline uint32_t size() const { return _size; };
private:
  uint32_t _buffer_id;
  uint32_t _size;
};

} // namespace Renderer

#endif // __VERTEX_BUFFER_H__
