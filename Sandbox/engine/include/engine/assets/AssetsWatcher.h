#ifndef __ASSETS_WATCHER_H__
#define __ASSETS_WATCHER_H__

#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <filesystem>

#include "engine/assets/IAssetsWatcher.h"

namespace engine {

class AssetsWatcher : IAssetsWatcher {
public:
  AssetsWatcher();
  virtual ~AssetsWatcher() override;

  virtual void start_watching() override;
  virtual void stop_watching() override;

  virtual void update() override;

private:  

  void watch_loop();

  bool _is_started = false;
  bool _stop_requested = false;
  
  std::unordered_map<std::string, std::filesystem::file_time_type> _last_assets_times;
  std::list<std::string> _dirty_list;
  std::mutex _dirty_list_mtx;

  std::thread _watcher_thread;
};

} // namespace engine

#endif // __ASSETS_WATCHER_H__
