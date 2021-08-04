#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>

namespace Renderer {

struct Transform {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  glm::vec3 rotation = glm::vec3(0.0f);

  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

  glm::mat4 toMatrix() const;
};

} // namespace Renderer

#endif // __TRANSFORM_H__
