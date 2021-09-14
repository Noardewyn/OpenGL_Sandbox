#ifndef __ASSETS_WATCHER_H__
#define __ASSETS_WATCHER_H__

#include <thread>
#include <unordered_map>
#include <chrono>
#include <filesystem>

#include "engine/assets/IAssetsWatcher.h"
#include "engine/assets/AssetManager.h"

namespace engine {

class AssetsWatcher : IAssetsWatcher {
public:
  AssetsWatcher();
  virtual ~AssetsWatcher() override;

  virtual void start_watching() override;
  virtual void stop_watching() override;

private:  

  void watch_loop();

  bool _is_started = false;
  bool _stop_requested = false;
  
  std::unordered_map<std::string, std::filesystem::file_time_type> _last_assets_times;
  std::thread _watcher_thread;
};

} // namespace engine

#endif // __ASSETS_WATCHER_H__
