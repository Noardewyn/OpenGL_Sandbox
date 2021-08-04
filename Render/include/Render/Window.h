#ifndef __RENDER_H__
#define __RENDER_H__

#include <iostream>

// OpenGL stuff
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

  inline double getDeltaTime() const { return _delta_time; }
  inline GLFWwindow* getHandle() const { return _glfw_handle; }

  inline uint32_t getWidth() const { return _width; }
  inline uint32_t getHeight() const { return _height; }

  inline double getCursorPosX() const { double x, y; glfwGetCursorPos(_glfw_handle, &x, &y); return x; }
  inline double getCursorPosY() const { double x, y; glfwGetCursorPos(_glfw_handle, &y, &y); return y; }

  inline void setCursorPos(double x, double y) const { return glfwSetCursorPos(_glfw_handle, x, y); }

private:
  uint32_t _width;
  uint32_t _height;
  std::string _title;

  GLFWwindow* _glfw_handle;

  double _delta_time = 0;
};

} // namespace Renderer

#endif // __RENDER_H__
