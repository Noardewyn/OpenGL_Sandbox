#include <GL/glew.h>

#include "Render/Logger.h"

#include "Render/FrameBuffer.h"

namespace Renderer {

  FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) {
    glGenFramebuffers(1, &_buffer_id);
    bind();

    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_id, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_CORE_ERROR("Framebuffer is not complete!");

    unbind();
  }

  FrameBuffer::~FrameBuffer() {
    if(_buffer_id != 0) {
      LOG_CORE_TRACE("Deleting FrameBuffer with id={}", _buffer_id);
      glDeleteFramebuffers(1, &_buffer_id);
    }
  }

  FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept {
    _buffer_id = other._buffer_id;
    other._buffer_id = 0;
  }

  FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) {
    if (this != &other) { 
      _buffer_id = other._buffer_id;
      other._buffer_id = 0;
    }

    return *this;
  }

  uint32_t FrameBuffer::getTexture() const {
    return _texture_id;
  }

  void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _buffer_id);
  }

  void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

} // namespace Renderer
