#include "engine/assets/ShaderAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  ShaderAsset::ShaderAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Shader, auto_reload_enabled) {
    
  }

  void ShaderAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    full_path.replace_extension();

    _shader = std::move(Renderer::Shader(full_path.string() + ".frag", full_path.string() + ".vs"));
  }

  void ShaderAsset::unload() {

  }

  void ShaderAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

  Renderer::Shader& ShaderAsset::getShader() {
    return _shader;
  }

} // namespace engine
