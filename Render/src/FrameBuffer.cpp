#include <GL/glew.h>

#include "Render/Logger.h"

#include "Render/FrameBuffer.h"

namespace Renderer {

  FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint32_t samples, bool only_depth)
  : _only_depth(only_depth) {
    generateBuffer(width, height, samples);
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

  uint32_t FrameBuffer::getId() const {
    return _buffer_id;
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

  void FrameBuffer::resize(uint32_t width, uint32_t height)
  {
    generateBuffer(width, height, _samples);
  }

  void FrameBuffer::resample(uint32_t samples)
  {
    generateBuffer(_width, _height, samples);
  }

  void FrameBuffer::generateBuffer(uint32_t width, uint32_t height, uint32_t samples /*= 0*/)
  {
    if(width == _width && _height == height && _samples == samples)
      return;
    
    _width = width;
    _height = height;
    _samples = samples;

    glGenFramebuffers(1, &_buffer_id);
    bind();
    glGenTextures(1, &_texture_id);

    if (samples) {
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture_id);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _texture_id, 0);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else {
      glBindTexture(GL_TEXTURE_2D, _texture_id);

      if(_only_depth) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
          width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture_id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
      }
      else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_id, 0);
      }

      glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_CORE_ERROR("Framebuffer is not complete!");

    unbind();
  }

} // namespace Renderer
