#include "engine/assets/AssetManager.h"

#include <algorithm>

namespace engine {
  std::string AssetManager::assetsPath() {
    return _assets_path;
  }

  void AssetManager::setAssetsPath(const std::string& path) {
    _assets_path = path;
  }

  Asset* AssetManager::loadAsset(const std::string& path, const Asset::AssetType& type) {
    auto asset = _assets.find(path);

    if (asset != _assets.end()) {
      
    }
    else {
      return asset->second.get();
    }
  }

  Asset* AssetManager::reloadAsset(const std::string& path, const Asset::AssetType& type) {

  }

  Asset* AssetManager::getAsset(const std::string& path, const Asset::AssetType& type) {

  }

  std::string AssetManager::_assets_path = "";
} // namespace engine
