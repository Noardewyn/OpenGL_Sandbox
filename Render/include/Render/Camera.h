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

  float moveSpeed = 5.0;
  float sensitivity = 0.1;
  float zoom_level = 45;

  Camera(const Transform &transform);

  glm::mat4 getViewMatrix() const;

  void move(glm::vec2 direction, double delta_time);
  void rotate(double xoffset, double yoffset, double zoffset, double delta_time);
  void zoom(double yoffset, double delta_time);

private:
  void updateCameraVectors();
};

} // namespace Renderer

#endif // __CAMERA_H__
