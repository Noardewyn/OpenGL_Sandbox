#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <iostream>

namespace Renderer {

class FrameBuffer {
public:
  FrameBuffer(uint32_t width, uint32_t height, uint32_t samples = 0);
  ~FrameBuffer();

  FrameBuffer(FrameBuffer&& other) noexcept;
  FrameBuffer& operator=(FrameBuffer&& other);

  uint32_t getId() const;
  uint32_t getTexture() const;

  void bind() const;
  void unbind() const;

  void resize(uint32_t width, uint32_t height);
  void resample(uint32_t samples = 0);

private:

  void generateBuffer(uint32_t width, uint32_t height, uint32_t samples = 0);

  uint32_t _texture_id = 0;
  uint32_t _buffer_id = 0;
  uint32_t _width = 0;
  uint32_t _height = 0;
  uint32_t _samples = 0;
};

} // namespace Renderer

#endif // __FRAME_BUFFER_H__
