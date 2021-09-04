#include "Render/Logger.h"

#include "Render/Window.h"

static void glfw_error_callback(int error, const char* description) {
  LOG_CORE_ERROR("GLFW Error {}: {}", error, description);
}

namespace Renderer {

  Window::Window(const uint32_t width, const uint32_t height, const std::string& title)
    : _size_windowed({0, 0}), _pos_windowed({0, 0}), _title(title) {

    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    _glfw_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (_glfw_handle == nullptr) {
      LOG_CORE_CRITICAL("Failed to create GLFW window");
      glfwTerminate();
      return;
    }
    
    _monitor = glfwGetPrimaryMonitor();

    glfwSetWindowUserPointer(_glfw_handle, this);
    glfwSetFramebufferSizeCallback(_glfw_handle, resizeCallback);
    glfwMakeContextCurrent(_glfw_handle);
    glfwSwapInterval(1);

    resize(width, height);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      LOG_CORE_CRITICAL("Failed to initialize GLEW");
      return;
    }

    const std::string versionString = std::string((const char*)glGetString(GL_VERSION));
    LOG_CORE_INFO("OpenGL version: {}", versionString);
  }

  Window::~Window() {
    glfwTerminate();
    LOG_CORE_TRACE("Window closed");
  }

  void Window::close() {
    glfwSetWindowShouldClose(_glfw_handle, GL_TRUE);
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

  void Window::display_start() {
    static double last_time = 0;
    double now = glfwGetTime();
    _delta_time = now - last_time;
    last_time = now;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Window::display_finish() {
    glfwSwapBuffers(_glfw_handle);
  }

  uint32_t Window::getWidth() const {
    int width, height;
    glfwGetFramebufferSize(_glfw_handle, &width, &height);
    return width;
  }

  uint32_t Window::getHeight() const {
    int width, height;
    glfwGetFramebufferSize(_glfw_handle, &width, &height);
    return height;
  }

  void Window::resize(int width, int height) {
    if(width == 0 || height == 0)
      return;

    _size_windowed[0] = width;
    _size_windowed[1] = height;

    const int aspect_width = int(((float)width / height) * height);
    const int aspect_height = int(((float)height / width) * width);

    glViewport(0, 0, aspect_width, aspect_height);
  }

  bool Window::isFullscreen() const {
    return glfwGetWindowMonitor(_glfw_handle) != nullptr;
  }

  void Window::setFullscreen(bool fullscreen) {
    if (isFullscreen() == fullscreen)
      return;

    if (fullscreen) {
      glfwGetWindowPos(_glfw_handle,  &_pos_windowed[0], &_pos_windowed[1]);
      glfwGetWindowSize(_glfw_handle, &_size_windowed[0], &_size_windowed[1]);
      glfwSwapInterval(1);

      const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      glfwSetWindowMonitor(_glfw_handle, _monitor, 0, 0, mode->width, mode->height, 0);
    }
    else {
      glfwSetWindowMonitor(_glfw_handle, nullptr, _pos_windowed[0], _pos_windowed[1], _size_windowed[0], _size_windowed[1], 0);
    }
  }

  void Window::setCursorEnabled(bool state) {
  if(state)
    glfwSetInputMode(_glfw_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  else
    glfwSetInputMode(_glfw_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  bool Window::isCursorEnabled() {
    return glfwGetInputMode(_glfw_handle, GLFW_CURSOR) != GLFW_CURSOR_DISABLED;
  }

  void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (auto* wnd_ptr = static_cast<Window*>(ptr)) {
      wnd_ptr->resize(width, height);
    }
  }

} // namespace Renderer
