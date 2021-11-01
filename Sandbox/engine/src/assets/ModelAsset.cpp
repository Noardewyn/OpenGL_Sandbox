#include "engine/assets/ModelAsset.h"

#include <filesystem>
#include "engine/assets/AssetManager.h"

namespace engine {

  ModelAsset::ModelAsset(const std::string& asset_path, bool auto_reload_enabled)
    : Asset::Asset(asset_path, AssetType::Model, auto_reload_enabled) {
    
  }

  void ModelAsset::load() {
    std::filesystem::path full_path(AssetManager::assetsPath() + getPath());
    _model = engine::Model(full_path.string());
  }

  void ModelAsset::unload() {

  }

  void ModelAsset::reload() {
    set_reloading(true);
    load();
    set_reloading(false);
  }

  void ModelAsset::draw(Renderer::Shader& shader) {
      _model.draw(shader);
  }

  void ModelAsset::draw(Renderer::Shader& shader, const Material& material) {
    _model.draw(shader, material);
  }

  engine::Model& ModelAsset::get() {
    return _model;
  }

} // namespace engine
