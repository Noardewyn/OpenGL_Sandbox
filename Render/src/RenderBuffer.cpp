#include <GL/glew.h>

#include "Render/Logger.h"

#include "Render/RenderBuffer.h"

namespace Renderer {

  RenderBuffer::RenderBuffer(uint32_t width, uint32_t height, uint32_t samples) { 
    generateBuffer(width, height, samples);
  }

  RenderBuffer::~RenderBuffer() {
    if(_buffer_id != 0) {
      LOG_CORE_TRACE("Deleting RenderBuffer with id={}", _buffer_id);
      glDeleteFramebuffers(1, &_buffer_id);
    }
  }

  RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept {
    _buffer_id = other._buffer_id;
    other._buffer_id = 0;
  }

  RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) {
    if (this != &other) { 
      _buffer_id = other._buffer_id;
      other._buffer_id = 0;
    }

    return *this;
  }

  void RenderBuffer::attach() const {
  }

  void RenderBuffer::bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, _buffer_id);
  }

  void RenderBuffer::unbind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  void RenderBuffer::resize(uint32_t width, uint32_t height)
  {
    generateBuffer(width, height, _samples);
  }

  void RenderBuffer::resample(uint32_t samples)
  {
    generateBuffer(_width, _height, samples);
  }

  void RenderBuffer::generateBuffer(uint32_t width, uint32_t height, uint32_t samples /*= 0*/)
  {
    if (width == _width && _height == height && _samples == samples)
      return;

    _width = width;
    _height = height;
    _samples = samples;

    glGenRenderbuffers(1, &_buffer_id);
    bind();

    if (samples) {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    }
    else {
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _buffer_id);
    unbind();
  }

} // namespace Renderer
