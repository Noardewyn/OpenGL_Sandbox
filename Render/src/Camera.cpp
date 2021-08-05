#include "Render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

Camera::Camera(const Transform& transform) 
  : transform(transform), world_up(0.0f, 1.0f, 0.0f) {
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

void Camera::move(glm::vec2 direction, double delta_time) {
  float velocity = moveSpeed * delta_time;

  transform.position += direction.y * transform.forward * velocity;
  transform.position += direction.x * transform.right * velocity;
}

void Camera::rotate(double xoffset, double yoffset, double zoffset, double delta_time) {
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

void Camera::zoom(double yoffset, double delta_time) {
  if (zoom_level >= 44.0f && zoom_level <= 47.0f)
    zoom_level -= yoffset * delta_time;
  if (zoom_level <= 44.0f)
    zoom_level = 44.0;
  if (zoom_level >= 47.0f)
    zoom_level = 47.0f;
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

} // namespace Renderer
