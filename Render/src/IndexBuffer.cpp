#include "Render/IndexBuffer.h"

#include <GL/glew.h>

namespace Renderer {

IndexBuffer::IndexBuffer()
  : _buffer_id(0), _count(0) {
  
}

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count) 
  : _buffer_id(0), _count(count) {
  glGenBuffers(1, &_buffer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  if(_buffer_id != 0) {
    std::cout << "IndexBuffer delete" << std::endl;
    glDeleteBuffers(1, &_buffer_id);
  }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {
  _buffer_id = other._buffer_id;
  _count = other._count;
  other._buffer_id = 0;
  other._count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) {
  if (this != &other) {
    _buffer_id = other._buffer_id;
    _count = other._count;
    other._buffer_id = 0;
    other._count = 0;
  }

  return *this;
}

void IndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
}

void IndexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Renderer
