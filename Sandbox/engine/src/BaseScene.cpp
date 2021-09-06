#include "engine/BaseScene.h"

#include "Render/Renderer.h"

namespace engine {

  BaseScene::BaseScene(Renderer::Window* window)
    : window(window) {
    Renderer::setDefaultParams();
    _assets_path = engine::AssetManager::assetsPath();
  }

  std::string BaseScene::assetsPath() {
    return _assets_path;
  }

} // namespace engine
