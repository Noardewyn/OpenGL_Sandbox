#include <GL/glew.h>

#include "Render/Renderer.h"

namespace Renderer {
  void CheckGLError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
      printf("after %s() glError (0x%x)\n", op, error);
    }
  }

  void DrawTriangles(const VertexArray& vao, const Shader &shader) {
    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, vao.vertex_count());
    CheckGLError("glDrawArrays");
  }

  void DrawTriangles(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.count(), GL_UNSIGNED_INT, 0);
    CheckGLError("glDrawElements");
  }
} // namespace Renderer
