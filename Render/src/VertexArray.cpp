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

} // namespace Renderer
