#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <iostream>

namespace Renderer {

class VertexBuffer {
public:
  VertexBuffer(const void* data, size_t size);
  ~VertexBuffer();

  VertexBuffer(VertexBuffer&& other) noexcept;
  VertexBuffer& operator=(VertexBuffer&& other);

  void bind() const;
  void unbind() const;

  inline size_t size() const { return _size; }
private:
  uint32_t _buffer_id;
  size_t _size;
};

} // namespace Renderer

#endif // __VERTEX_BUFFER_H__
