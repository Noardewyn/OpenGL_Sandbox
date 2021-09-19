#include "engine/assets/MeshAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  MeshAsset::MeshAsset(const std::string& asset_path, bool auto_reload_enabled)
    : _asset_path(asset_path), _type(AssetType::Mesh), _auto_reload_enabled(auto_reload_enabled) {
    
  }

  void MeshAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    _mesh = engine::Mesh(full_path.c_str());
  }

  void MeshAsset::unload() {

  }

  void MeshAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

} // namespace engine
