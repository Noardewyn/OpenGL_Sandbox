#include "Render/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

Transform::Transform() {

}

Transform::Transform(float x, float y, float z)
  : position(glm::vec3(x, y, z)) {
  
}

glm::mat4 Transform::toMatrix() const {
  glm::mat4 model(1.0f);
  model = glm::translate(model, glm::vec3(position.x, position.y, position.z));

  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  model = glm::scale(model, scale);

  return model;
}

} // namespace Renderer
