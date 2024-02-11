#include <GL/glew.h>

#include "Render/Logger.h"

#include "Render/VertexArray.h"

namespace Renderer {

  VertexArray::VertexArray() 
    :_vertex_count(0) {
    glGenVertexArrays(1, &_array_id);
    glBindVertexArray(_array_id);
  }

  VertexArray::~VertexArray() {
    LOG_CORE_TRACE("Deleting VertexArray with id={}", _array_id);
  }

  void VertexArray::bind() const {
    glBindVertexArray(_array_id);
  }

  void VertexArray::unbind() const {
    glBindVertexArray(0);
  }

  void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) {
    bind();
    vbo.bind();

    int index = 0;
    size_t offset = 0;

    for (const auto &element : layout.getElements()) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<const void*>(offset));
      
      offset += element.count * VertexBufferElement::getTypeSize(element.type);
      ++index;
    }

    _vertex_count = vbo.size() / offset;
  }

} // namespace Renderer
