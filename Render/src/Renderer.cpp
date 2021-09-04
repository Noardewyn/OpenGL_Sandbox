#include <GL/glew.h>

#include "Render/Renderer.h"

namespace Renderer {
  void CheckGLError(const char* func, const char* file, int line) {
    for (GLint error = glGetError(); error; error = glGetError()) {
      LOG_CORE_ERROR("GL_ERROR errcode={}\n\tfunc={} file={} line={}", error, func, file, line);
    }
  }

  void DrawTriangles(const VertexArray& vao, const Shader &shader) {
    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, vao.vertex_count());
    CHECK_OPENGL_ERROR();
  }

  void DrawTriangles(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.count(), GL_UNSIGNED_INT, 0);
    CHECK_OPENGL_ERROR();
  }

  void setDefaultParams() {
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }

} // namespace Renderer
