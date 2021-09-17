#include "engine/assets/TextureAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  TextureAsset::TextureAsset(const std::string& asset_path, bool auto_reload_enabled)
    : _asset_path(asset_path), _type(AssetType::Texture), _auto_reload_enabled(auto_reload_enabled) {
    
  }

  void TextureAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    _texture = Render::Texture(full_path.c_str());
  }

  void TextureAsset::unload() {

  }

  void TextureAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

} // namespace engine
