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

  void AssetsWatcher::watch_loop() {
    while (!_stop_requested) {
      auto& assets = AssetManager::getAssets();

      for (auto& asset : assets) {
        namespace fs = std::filesystem;

        const std::string& path_str = asset->getPath();
        const fs::path path = asset->getPath();

        if (!fs::exists(path)) {
          LOG_CORE_ERROR("Watched asset is not exists: {}", path_str);
          continue;
        }

        // not in cache
        if (_last_assets_times.find(path_str) != _last_assets_times.end()) {
          _last_assets_times[path_str] = fs::last_write_time(path);
          LOG_CORE_INFO("Start watching: {}", path_str);
          continue;
        }

        const auto current_time = fs::last_write_time(path);

        // if file changed
        if (_last_assets_times[path_str] != current_time) {
          LOG_CORE_INFO("Reload asset: {}", path_str);
          asset->reload();
          _last_assets_times[path_str] = fs::last_write_time(path);
        }
      }
    }
  }

} // namespace engine
