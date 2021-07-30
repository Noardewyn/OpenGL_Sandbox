#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include <iostream>

namespace Renderer {

class IndexBuffer {
public:
  IndexBuffer(const uint32_t* data, uint32_t count);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

private:
  uint32_t _buffer_id;
};

} // namespace Renderer

#endif // __INDEX_BUFFER_H__
