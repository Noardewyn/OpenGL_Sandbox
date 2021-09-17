#ifndef __I_ASSETS_WATCHER_H__
#define __I_ASSETS_WATCHER_H__

namespace engine {

  class IAssetsWatcher {
  public:
    virtual ~IAssetsWatcher() = default;
    virtual void start_watching() = 0;
    virtual void stop_watching() = 0;
    virtual void update() = 0;
  };

} // namespace engine

#endif // __I_ASSETS_WATCHER_H__
