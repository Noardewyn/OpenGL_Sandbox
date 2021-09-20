#include "engine/assets/TextureAsset.h"

#include <vector>
#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  TextureAsset::TextureAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Texture, auto_reload_enabled) {
    
  }

  TextureAsset::TextureAsset(const std::string& name, const std::vector<std::string>& cubemap, bool auto_reload_enabled)
    : Asset::Asset(name, AssetType::Texture, auto_reload_enabled) {
    std::vector<std::string> full_pathes;

    for (auto path : cubemap) {
      full_pathes.push_back(AssetManager::assetsPath() + path);
    }

    _texture = Renderer::Texture(full_pathes);
  }

  void TextureAsset::load() {
    if (!_texture.is_cubemap) {
      std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
      _texture = Renderer::Texture(full_path.string());
    }
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
