#include "Render/VertexArray.h"

#include <GL/glew.h>

namespace Renderer {

  VertexArray::VertexArray() {
    glGenVertexArrays(1, &_array_id);
    glBindVertexArray(_array_id);
  }

  VertexArray::~VertexArray() {

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
    uint32_t offset = 0;

    for (const auto &element : layout.getElements()) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
      
      offset += element.count * VertexBufferElement::getTypeSize(element.type);
      ++index;
    }
  }

} // namespace Renderer
