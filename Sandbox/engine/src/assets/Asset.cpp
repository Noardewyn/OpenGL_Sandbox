#include "engine/assets/Asset.h"

namespace engine {

  Asset::Asset(const std::string& asset_path, const AssetType& type, bool auto_reload_enabled)
    : _asset_path(asset_path), _type(type), _auto_reload_enabled(auto_reload_enabled) {
    
  }

  Asset::AssetType Asset::getType() const {
    return _type;
  }

  std::string Asset::getPath() const {
    return _asset_path;
  }

  bool Asset::is_auto_reload_enabled() const {
    return _auto_reload_enabled;
  }

  void Asset::set_auto_reload_enabled(bool enabled) {
    _auto_reload_enabled = enabled;
  }

  bool Asset::set_reloading(bool state) {
    _is_reloading = state;
  }

  bool Asset::is_reloading() const {
    return _is_reloading;
  }

} // namespace engine
