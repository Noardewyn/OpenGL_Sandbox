#include "engine/assets/ShaderAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  ShaderAsset::ShaderAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Shader, auto_reload_enabled) {
    
  }

  void ShaderAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    _shader = Renderer::Shader(full_path.string());
  }

  void ShaderAsset::unload() {

  }

  void ShaderAsset::reload() {
    set_reloading(true);
    _shader.recompile();
    set_reloading(false);
  }

  Renderer::Shader& ShaderAsset::get() {
    return _shader;
  }

} // namespace engine
