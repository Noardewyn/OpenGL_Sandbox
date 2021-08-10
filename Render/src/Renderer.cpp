#include <GL/glew.h>

#include "Render/Renderer.h"

namespace Renderer {
  void DrawTriangles(const VertexArray& vao, const Shader &shader) {
    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, vao.vertex_count());
    vao.unbind();
    shader.unbind();
  }

  void DrawTriangles(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.count(), GL_UNSIGNED_INT, 0);
    vao.unbind();
    shader.unbind();
  }
} // namespace Renderer
