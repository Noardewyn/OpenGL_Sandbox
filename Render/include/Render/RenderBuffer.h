#ifndef __RENDER_BUFFER_H__
#define __RENDER_BUFFER_H__

#include <iostream>

#include "Render/RenderBuffer.h"

namespace Renderer {

class RenderBuffer {
public:
  RenderBuffer(uint32_t width, uint32_t height, uint32_t samples = 0);
  ~RenderBuffer();

  RenderBuffer(RenderBuffer&& other) noexcept;
  RenderBuffer& operator=(RenderBuffer&& other);

  void attach() const;
  void bind() const;
  void unbind() const;

  void resize(uint32_t width, uint32_t height);
  void resample(uint32_t samples = 0);
private:

  void generateBuffer(uint32_t width, uint32_t height, uint32_t samples = 0);

  uint32_t _buffer_id = 0;
  uint32_t _width = 0;
  uint32_t _height = 0;
  uint32_t _samples = 0;
};

} // namespace Renderer

#endif // __RENDER_BUFFER_H__
