#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "Render/Window.h"

namespace engine {

class BaseScene {
public:
  BaseScene(Renderer::Window *window);
  virtual ~BaseScene() = default;

  virtual void onUpdate(float delta_time) {}
  virtual void onRender() {}
  virtual void onImGuiRender() {}

  Renderer::Window *window;
};

} // namespace engine

#endif // __BASE_SCENE_H__
