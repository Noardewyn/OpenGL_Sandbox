#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "Render/Window.h"
#include "engine/assets/AssetManager.h"

#include <map>

namespace engine {

enum class DebugMode { ShowNormalMaps };
enum class DebugDisplayMode { Default, Normal_TS, Normal_VS, NormalTexture, Tangent, Bitangent, Count };

class BaseScene {
public:
  BaseScene(Renderer::Window *window);
  virtual ~BaseScene() = default;

  virtual void onUpdate(float delta_time) {}
  virtual void onRender() {}
  virtual void onImGuiRender() {}

  std::string assetsPath();

  void setDebugMode(const DebugMode& mode, bool state);
  bool getDebugMode(const DebugMode& mode);

  void setDebugDisplayMode(const DebugDisplayMode& mode);
  DebugDisplayMode getDebugDisplayMode();

  Renderer::Window *window;

protected:
  DebugDisplayMode _debug_display_mode = DebugDisplayMode::Default;

private:  
  void init_debug_modes();
  
  std::string _assets_path;
  std::unordered_map<DebugMode, bool> _debug_modes;
};

} // namespace engine

#endif // __BASE_SCENE_H__
