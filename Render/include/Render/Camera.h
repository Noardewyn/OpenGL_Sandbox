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

  float moveSpeed = 5.0f;
  float sensitivity = 0.1f;
  float fov = 45.0f;

  float near_plane = 0.1f;
  float far_plane = 200.0f;

  Camera();
  Camera(const Transform &transform, const glm::mat4 &projection = glm::mat4(1));

  void setPerspective(float aspect);
  void setOrtho(float width, float height);
  void setProjection(const glm::mat4 &matrix);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;

  glm::mat4 getViewProjectionMatrix() const;
   
  void move(glm::vec2 direction, float delta_time);
  void rotate(float xoffset, float yoffset, float zoffset, float delta_time);
  void zoom(float yoffset, float delta_time);

private:
  glm::mat4 _projection;


  void updateCameraVectors();
};

} // namespace Renderer

#endif // __CAMERA_H__
