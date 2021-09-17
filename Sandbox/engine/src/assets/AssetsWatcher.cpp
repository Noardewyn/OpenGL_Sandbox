#include "engine/assets/AssetsWatcher.h"

#include <functional>

#include "Render/Logger.h"

namespace engine {

  AssetsWatcher::AssetsWatcher() {

  }

  AssetsWatcher::~AssetsWatcher() {
    stop_watching();
  }

  void AssetsWatcher::start_watching() {
    _stop_requested = false;
    _watcher_thread = std::thread([this](){ return watch_loop(); });
    _is_started = true;
  }

  void AssetsWatcher::stop_watching() {
    _stop_requested = true;
    _watcher_thread.join();
    _is_started = false;
  }

  void AssetsWatcher::update() {
    if (!_dirty_list.empty()) {
      std::lock_guard lg(_dirty_list_mtx);

      for (auto& it = _dirty_list.begin(); it != _dirty_list.end(); it++) {
        const std::string& path_str = *it;
        Asset* asset = AssetManager::getAsset<Asset>(path_str);

        if (asset) {
          if (asset->is_auto_reload_enabled()) {
            LOG_CORE_INFO("Reload asset: {}", path_str);
            asset->reload();
          }
        }
        else {
          LOG_CORE_ERROR("Reload asset error - asset not found: {}", path_str);
        }
      }

      _dirty_list.clear();
    }
  }

  void AssetsWatcher::watch_loop() {
    while (!_stop_requested) {
      auto& assets = AssetManager::getAssets();

      for (auto& asset : assets) {
        namespace fs = std::filesystem;

        std::string base_path_str = asset->getPath();
        std::string full_path_str = AssetManager::assetsPath() + base_path_str;

        fs::path path(full_path_str);

        if (path.extension() == ".glsl") {
          path.replace_extension(".frag");
          full_path_str = path.string();
          full_path_str.erase(full_path_str.find(AssetManager::assetsPath()), AssetManager::assetsPath().size());
        }

        if (!fs::exists(path)) {
          LOG_CORE_ERROR("Watched asset is not exists: {}", base_path_str);
          continue;
        }

        // not in cache
        if (_last_assets_times.find(base_path_str) == _last_assets_times.end()) {
          _last_assets_times[base_path_str] = fs::last_write_time(path);
          LOG_CORE_INFO("Start watching: {}", base_path_str);
          continue;
        }

        const auto current_time = fs::last_write_time(path);

        // if file changed
        if (_last_assets_times[base_path_str] != current_time) {
          LOG_CORE_INFO("Asset was changed: {}", base_path_str);
          _last_assets_times[base_path_str] = fs::last_write_time(path);
          std::lock_guard lg(_dirty_list_mtx);
          _dirty_list.push_back(base_path_str);
        }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

} // namespace engine
