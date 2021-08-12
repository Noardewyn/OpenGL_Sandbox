#include <GL/glew.h>

#include "Render/Logger.h"

#include "Render/VertexBuffer.h"

namespace Renderer {

  VertexBuffer::VertexBuffer(const void* data, size_t size)
    : _size(size) {
    glGenBuffers(1, &_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  }

  VertexBuffer::~VertexBuffer() {
    if(_buffer_id != 0) {
      LOG_CORE_TRACE("Deleting VertexBuffer with id={}", _buffer_id);
      glDeleteBuffers(1, &_buffer_id);
    }
  }

  VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
    _buffer_id = other._buffer_id;
    _size = other._size;
    other._buffer_id = 0;
    other._size = 0;
  }

  VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    if (this != &other) {
      _buffer_id = other._buffer_id;
      _size = other._size;
      other._buffer_id = 0;
      other._size = 0;
    }

    return *this;
  }

  void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
  }

  void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

} // namespace Renderer
