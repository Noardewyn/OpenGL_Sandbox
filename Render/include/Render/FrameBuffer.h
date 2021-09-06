#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <iostream>

namespace Renderer {

class FrameBuffer {
public:
  FrameBuffer(uint32_t width, uint32_t height);
  ~FrameBuffer();


  FrameBuffer(FrameBuffer&& other) noexcept;
  FrameBuffer& operator=(FrameBuffer&& other);

  uint32_t getTexture() const;
  void bind() const;
  void unbind() const;

private:
  uint32_t _texture_id;
  uint32_t _buffer_id;
};

} // namespace Renderer

#endif // __FRAME_BUFFER_H__
