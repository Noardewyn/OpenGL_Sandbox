#include "Render/IndexBuffer.h"

#include <GL/glew.h>

namespace Renderer {

  IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count) {
    glGenBuffers(1, &_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
  }

  IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &_buffer_id);
  }

  void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
  }

  void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

} // namespace Renderer
