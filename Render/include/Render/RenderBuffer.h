#ifndef __RENDER_BUFFER_H__
#define __RENDER_BUFFER_H__

#include <iostream>

#include "Render/RenderBuffer.h"

namespace Renderer {

class RenderBuffer {
public:
  RenderBuffer(uint32_t width, uint32_t height);
  ~RenderBuffer();

  RenderBuffer(RenderBuffer&& other) noexcept;
  RenderBuffer& operator=(RenderBuffer&& other);

  void attach() const;
  void bind() const;
  void unbind() const;

private:
  uint32_t _buffer_id;
};

} // namespace Renderer

#endif // __RENDER_BUFFER_H__
