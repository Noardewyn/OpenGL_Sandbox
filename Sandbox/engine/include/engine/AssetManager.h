#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__

#include <string>

namespace engine {

class AssetManager {
public:
  static std::string assetsPath();
  static void setAssetsPath(const std::string &path);

private:
  AssetManager() = default;
  static std::string _assets_path;
};

} // namespace engine

#endif // __ASSET_MANAGER_H__
