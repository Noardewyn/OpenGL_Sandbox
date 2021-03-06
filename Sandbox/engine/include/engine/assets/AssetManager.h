#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__

#include <string>
#include <unordered_map>
#include <memory>
#include <initializer_list>

#include "Render/Logger.h"
#include "engine/assets/Asset.h"
#include "engine/assets/ShaderAsset.h"

namespace engine {

class AssetManager {
public:
  static std::string assetsPath();
  static void setAssetsPath(const std::string &path);
  
  template<typename T>
  static T* loadAsset(const std::string& path, bool auto_reloading = false) {
    auto asset = _assets.find(path);

    if (asset != _assets.end()) {
      return dynamic_cast<T*>(asset->second.get());
    }
    else {
      _assets[path] = std::unique_ptr<T>(new T(path, auto_reloading));
      LOG_CORE_INFO("[AssetsManager] New asset loaded: {}", path);
      _assets[path]->load();

      return dynamic_cast<T*>(_assets[path].get());
    }
  }

  template<typename T>
  static T* addAsset(T* asset) {
    assert(asset != nullptr);

    auto path = asset->getPath();
    auto asset_ptr = _assets.find(path);

    if (asset_ptr != _assets.end()) {
      return dynamic_cast<T*>(asset_ptr->second.get());
    }
    else {
      _assets[path] = std::unique_ptr<T>(asset);
      LOG_CORE_INFO("[AssetsManager] Asset added: {}", path);
      return dynamic_cast<T*>(_assets[path].get());
    }
  }

  template<typename T>
  static T* reloadAsset(const std::string& path) {

  }

  static bool hasAsset(const std::string& path) {
    auto asset_ptr = _assets.find(path);

    if (asset_ptr != _assets.end()) {
      return true;
    }
    else {
      if (path.find(AssetManager::assetsPath()) != std::string::npos) {
        return hasAsset(truncateBasePath(path));
      }
    }

    return false;
  }

  template<typename T>
  static T* getAsset(const std::string& path) {
    auto asset_ptr = _assets.find(path);
    
    if (asset_ptr != _assets.end()) {
      return dynamic_cast<T*>(asset_ptr->second.get());
    }
    else {
      if (path.find(AssetManager::assetsPath()) != std::string::npos) {
        return getAsset<T>(truncateBasePath(path));
      }
    }

    return nullptr;
  }

  static std::vector<Asset*> getAssets();

  static std::string truncateBasePath(const std::string& path);

private:
  AssetManager() = default;
  static std::string _assets_path;
  static std::unordered_map<std::string, std::unique_ptr<Asset>> _assets;
};


} // namespace engine

#endif // __ASSET_MANAGER_H__
