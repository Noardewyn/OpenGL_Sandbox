#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>
#include <glm/glm.hpp>

#include "Render/Transform.h"

namespace Renderer {

class Camera {
public:
  Transform transform;
  glm::vec3 world_up;

  float moveSpeed = 3.0;
  float sensitivity = 0.1;
  float zoom_level = 45;

  Camera(const Transform &transform);

  glm::mat4 getViewMatrix() const;

  void move(glm::vec2 direction, float delta_time);
  void rotate(float xoffset, float yoffset, float zoffset, float delta_time);
  void zoom(float yoffset);

private:
  void updateCameraVectors();
};

} // namespace Renderer

#endif // __CAMERA_H__
