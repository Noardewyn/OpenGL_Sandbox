﻿#include "Render/Window.h"

float last_time = 0;

Window::Window(const uint32_t width, const uint32_t height, const std::string& title)
  : _width(width), _height(height), _title(title) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  _glfw_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (_glfw_handle == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }
  
  glfwMakeContextCurrent(_glfw_handle);
}

Window::~Window() {
  close();
}

void Window::close() {
  glfwTerminate();
  std::cout << "Window closed" << std::endl;
}

void Window::clear(const Color& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

bool Window::isOpen() const {
  return !glfwWindowShouldClose(_glfw_handle);
}

void Window::pollEvents() {
  glfwPollEvents();
}

void Window::display() {
  double now = glfwGetTime();
  _delta_time = now - last_time;
  last_time = now;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwSwapBuffers(_glfw_handle);
}