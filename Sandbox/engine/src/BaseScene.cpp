#include "engine/BaseScene.h"

#include "Render/Renderer.h"

namespace engine {

  BaseScene::BaseScene(Renderer::Window* window)
    : window(window) {
    Renderer::setDefaultParams();
  }
} // namespace engine
