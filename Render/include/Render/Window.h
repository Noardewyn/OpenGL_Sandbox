#ifndef __RENDER_H__
#define __RENDER_H__

#include <iostream>

// OpenGL stuff
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Render/Color.h"

class Window {
public:
  Window(const uint32_t width, const uint32_t height, const std::string& title);
  ~Window();

  bool isOpen() const;
  void clear(const Color &color);
  void display();
  void close();
  void pollEvents();

private:
  uint32_t _width;
  uint32_t _height;
  std::string _title;

  GLFWwindow* _glfw_handle;

  double _delta_time = 0;
};

#endif // __RENDER_H__
