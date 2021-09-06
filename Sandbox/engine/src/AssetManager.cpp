#include "engine/AssetManager.h"

namespace engine {
  std::string AssetManager::assetsPath() {
    return _assets_path;
  }

  void AssetManager::setAssetsPath(const std::string& path) {
    _assets_path = path;
  }

  std::string AssetManager::_assets_path = "";
} // namespace engine
