#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__

#include <string>
#include <unordered_map>
#include <memory>

#include "engine/assets/Asset.h"

namespace engine {

class AssetManager {
public:
  static std::string assetsPath();
  static void setAssetsPath(const std::string &path);
  
  static Asset* loadAsset(const std::string& path, const Asset::AssetType& type);
  static Asset* reloadAsset(const std::string& path, const Asset::AssetType& type);
  static Asset* getAsset(const std::string& path, const Asset::AssetType& type);

private:
  AssetManager() = default;
  static std::string _assets_path;
  static std::unordered_map<std::string, std::unique_ptr<Asset>> _assets;
};

} // namespace engine

#endif // __ASSET_MANAGER_H__
