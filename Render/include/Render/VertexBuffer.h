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

private:
  uint32_t _buffer_id;
};

} // namespace Renderer

#endif // __VERTEX_BUFFER_H__
