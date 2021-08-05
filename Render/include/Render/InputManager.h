#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "Render/Window.h"

namespace tools {

class InputManager {
public:
  static void init(const Renderer::Window& window);
  static InputManager * const instance();

  bool keyPressed(int key_code) const;
  bool keyDown(int key_code) const;
  double mousePosX() const;
  double mousePosY() const;
  double wheelX() const;
  double wheelY() const;

private:
  static std::unique_ptr<InputManager> _instance;

  InputManager(const Renderer::Window& window);
};

} // namespace tools

#endif // __INPUT_MANAGER_H__
