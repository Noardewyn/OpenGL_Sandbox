#include "engine/assets/TextureAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  TextureAsset::TextureAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Texture, auto_reload_enabled) {
    
  }

  void TextureAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    _texture = Renderer::Texture(full_path.string());
  }

  void TextureAsset::unload() {

  }

  void TextureAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

  Renderer::Texture& TextureAsset::get() {
    return _texture;
  }

} // namespace engine
