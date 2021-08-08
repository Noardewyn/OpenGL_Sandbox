#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include <iostream>

namespace Renderer {

class IndexBuffer {
public:
  IndexBuffer();
  IndexBuffer(const uint32_t* data, uint32_t count);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

  inline uint32_t count() const { return _count; };
private:
  uint32_t _buffer_id;
  uint32_t _count;
};

} // namespace Renderer

#endif // __INDEX_BUFFER_H__
