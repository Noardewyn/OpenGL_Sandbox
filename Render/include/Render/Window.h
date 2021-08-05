#ifndef __RENDER_H__
#define __RENDER_H__

#include <iostream>
#include <array>

// OpenGL stuff
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Render/Color.h"

namespace Renderer {

class Window {
public:
  Window(const uint32_t width, const uint32_t height, const std::string& title);
  ~Window();

  bool isOpen() const;
  void clear(const Color &color);
  void display_start();
  void display_finish();
  void close();
  void pollEvents();
  
  void setCursorEnabled(bool state);
  bool isCursorEnabled();

  uint32_t getWidth() const;
  uint32_t getHeight() const;

  void resize(int width, int height);
  bool isFullscreen() const;
  void setFullscreen(bool fullscreen);

  inline double getDeltaTime() const { return _delta_time; }
  inline GLFWwindow* getHandle() const { return _glfw_handle; }

  inline double getCursorPosX() const { double x, y; glfwGetCursorPos(_glfw_handle, &x, &y); return x; }
  inline double getCursorPosY() const { double x, y; glfwGetCursorPos(_glfw_handle, &y, &y); return y; }

  inline void setCursorPos(double x, double y) const { return glfwSetCursorPos(_glfw_handle, x, y); }

private:
  std::array<int, 2> _size_windowed;
  std::array<int, 2> _pos_windowed;

  std::string _title;

  GLFWmonitor *_monitor;
  GLFWwindow  *_glfw_handle;

  double _delta_time = 0;

  static void resizeCallback(GLFWwindow* window, int width, int height);
};

} // namespace Renderer

#endif // __RENDER_H__
