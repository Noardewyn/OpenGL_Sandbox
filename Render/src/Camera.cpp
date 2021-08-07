#include "Render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

Camera::Camera(const Transform& transform, const glm::mat4& projection)
  : _projection(projection), transform(transform),
    world_up(0.0f, 1.0f, 0.0f) {
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

glm::mat4 Camera::getProjectionMatrix() const {
  return _projection;
}

glm::mat4 Camera::getViewProjectionMatrix() const {
  return _projection * getViewMatrix();
}

void Camera::move(glm::vec2 direction, float delta_time) {
  const float velocity = moveSpeed * delta_time;

  transform.position += direction.y * transform.forward * velocity;
  transform.position += direction.x * transform.right * velocity;
}

void Camera::rotate(float xoffset, float yoffset, float zoffset, float delta_time) {
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  transform.rotation.x += xoffset;
  transform.rotation.y += yoffset;

  if (transform.rotation.y > 89.0f)
    transform.rotation.y = 89.0f;
  if (transform.rotation.y < -89.0f)
    transform.rotation.y = -89.0f;

  updateCameraVectors();
}

void Camera::zoom(float yoffset, float delta_time) {
  if (fov >= 44.0f && fov <= 47.0f)
    fov -= yoffset * delta_time;
  if (fov <= 44.0f)
    fov = 44.0;
  if (fov >= 47.0f)
    fov = 47.0f;
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
  front.y = sin(glm::radians(transform.rotation.y));
  front.z = sin(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
  transform.forward = glm::normalize(front);

  transform.right = glm::normalize(glm::cross(transform.forward, world_up));
  transform.up = glm::normalize(glm::cross(transform.right, transform.forward));
}

void Camera::setPerspective(float aspect) {
  _projection = glm::perspective(fov, aspect, near_plane, far_plane);
}

void Camera::setOrtho(float width, float height) {
  float aspect = height / width;
  _projection = glm::ortho(0.0f, 1.0f, -1.0f * aspect, 1.0f * aspect, near_plane, far_plane);
}

} // namespace Renderer
