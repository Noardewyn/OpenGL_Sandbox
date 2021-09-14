#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__

#include <string>
#include <unordered_map>
#include <memory>

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
      _assets[path]->load();

      return dynamic_cast<T*>(_assets[path].get());
    }
  }

  template<typename T>
  static T* reloadAsset(const std::string& path) {

  }

  template<typename T>
  static T* getAsset(const std::string& path) {

  }

  static std::vector<Asset*> getAssets();

private:
  AssetManager() = default;
  static std::string _assets_path;
  static std::unordered_map<std::string, std::unique_ptr<Asset>> _assets;
};


} // namespace engine

#endif // __ASSET_MANAGER_H__
