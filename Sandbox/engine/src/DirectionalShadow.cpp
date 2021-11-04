#include "engine/DirectionalShadow.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

  DirectionalShadow::DirectionalShadow(int width, int height)
  {
    options.width = width;
    options.height = height;

    _frame_buffer = std::make_unique<Renderer::FrameBuffer>(width, height, 0, true);
  }

  glm::mat4 DirectionalShadow::getLigthMatrix(const glm::vec3& direction)
  {
    float near_plane = 0.001f, far_plane = 300.0f;
    glm::mat4 light_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    //glm::mat4 light_projection = glm::perspective(45.0f, 1.0f, near_plane, far_plane);

    glm::mat4 light_view = glm::lookAt(-direction * 100.0f,
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));

    return light_projection * light_view;
  }

  void DirectionalShadow::prepare()
  {
    glViewport(0, 0, options.width, options.height);
    _frame_buffer->bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
  }

  void DirectionalShadow::complete()
  {
    glCullFace(GL_BACK);
    _frame_buffer->unbind();
  }

  uint32_t DirectionalShadow::getShadowMapId()
  {
    return _frame_buffer->getTexture();
  }

} // namespace engine
