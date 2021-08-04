#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Render/InputManager.h"

static bool keys[1024];
static double mouse_pos_x;
static double mouse_pos_y;
static double wheel_x;
static double wheel_y;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  mouse_pos_x = xpos;
  mouse_pos_y = ypos;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  wheel_x = xoffset;
  wheel_y = yoffset;
}

namespace tools {

std::unique_ptr<InputManager> InputManager::_instance = nullptr;

InputManager* const InputManager::instance() {
  assert(_instance && "Input manager must be inited first");
  return _instance.get();
}

void InputManager::init(const Renderer::Window& window) {
  assert(window.getHandle() && window.isOpen());
  _instance = std::unique_ptr<InputManager>(new InputManager(window));
}

InputManager::InputManager(const Renderer::Window& window) {
  glfwSetKeyCallback(window.getHandle(), key_callback);
  glfwSetCursorPosCallback(window.getHandle(), mouse_callback);
  glfwSetScrollCallback(window.getHandle(), scroll_callback);
}

bool InputManager::keyPressed(int key_code) const {
  return keys[key_code] == GLFW_PRESS;
}

double InputManager::mousePosX() const {
  return mouse_pos_x;
}

double InputManager::mousePosY() const {
  return mouse_pos_y;
}

double InputManager::wheelX() const {
  return wheel_x;
}

double InputManager::wheelY() const {
  return wheel_y;
}

} // namespace Renderer
