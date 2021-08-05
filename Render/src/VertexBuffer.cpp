#include "Render/VertexBuffer.h"

#include <GL/glew.h>

namespace Renderer {

  VertexBuffer::VertexBuffer(const void* data, uint32_t size) 
    : _size(size) {
    glGenBuffers(1, &_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  }

  VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &_buffer_id);
  }

  void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
  }

  void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

} // namespace Renderer
