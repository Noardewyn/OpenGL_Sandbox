#include "engine/assets/MaterialAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  MaterialAsset::MaterialAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Material, auto_reload_enabled) {
    
  }

  void MaterialAsset::load() {
    _material = engine::Material(getPath());
  }

  void MaterialAsset::unload() {

  }

  void MaterialAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

  engine::Material& MaterialAsset::get() {
    return _material;
  }

} // namespace engine
