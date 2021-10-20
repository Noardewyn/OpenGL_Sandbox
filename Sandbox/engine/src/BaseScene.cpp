#include "engine/BaseScene.h"

#include "Render/Renderer.h"

namespace engine {

  BaseScene::BaseScene(Renderer::Window* window)
    : window(window) {
    Renderer::setDefaultParams();
    _assets_path = engine::AssetManager::assetsPath();
    init_debug_modes();
  }

  std::string BaseScene::assetsPath() {
    return _assets_path;
  }

  void BaseScene::init_debug_modes()
  {
      _debug_modes[DebugMode::ShowNormalMaps] = false;
  }

  void BaseScene::setDebugMode(const DebugMode& mode, bool state)
  {
      _debug_modes[mode] = state;
  }

  bool BaseScene::getDebugMode(const DebugMode& mode)
  {
    return _debug_modes[mode];
  }

  void BaseScene::setDebugDisplayMode(const DebugDisplayMode& mode)
  {
    _debug_display_mode = mode;
  }

  engine::DebugDisplayMode BaseScene::getDebugDisplayMode()
  {
    return _debug_display_mode;
  }

} // namespace engine
