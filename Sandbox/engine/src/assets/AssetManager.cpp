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

  std::vector<Asset*> AssetManager::getAssets() {
    std::vector<Asset*> assets;

    for (const auto &[path, asset_ptr] : _assets) {
      assets.push_back(asset_ptr.get());
    }

    return assets;
  }

  std::string AssetManager::_assets_path = "";
  std::unordered_map<std::string, std::unique_ptr<Asset>> AssetManager::_assets;

} // namespace engine
