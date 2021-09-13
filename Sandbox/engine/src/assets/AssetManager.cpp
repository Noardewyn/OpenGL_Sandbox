#include "engine/assets/AssetManager.h"

#include <algorithm>

#include "engine/assets/ShaderAsset.h"

namespace engine {
  std::string AssetManager::assetsPath() {
    return _assets_path;
  }

  void AssetManager::setAssetsPath(const std::string& path) {
    _assets_path = path;
  }

  std::string AssetManager::_assets_path = "";
  std::unordered_map<std::string, std::unique_ptr<Asset>> AssetManager::_assets;

} // namespace engine
