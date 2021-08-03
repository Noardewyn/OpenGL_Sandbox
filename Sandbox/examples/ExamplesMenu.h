#ifndef __EXAMPLES_MENU_H__
#define __EXAMPLES_MENU_H__

#include <unordered_map>
#include <functional>
#include "BaseScene.h"

namespace Sandbox {

struct SceneMetadata {
  std::string name; 
  std::string description;
  std::function<std::unique_ptr<BaseScene> (Renderer::Window* w)> build_function;
};

class ExamplesMenu : public BaseScene {
public:
  ExamplesMenu(Renderer::Window* window);
  virtual ~ExamplesMenu() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

  BaseScene *getCurrentScene() { return _current_scene; }
  void startScene(int index);

  inline float scenesCount() { return _scenes.size(); };

  inline void addSceneMetadata(const SceneMetadata& metadata) {
    _scenes.push_back(metadata);
  }

  template<typename T>
  inline void registerScene(const std::string &name, const std::string& description) {
    const SceneMetadata &metadata = 
      { name, description, [](Renderer::Window* w) { return std::make_unique<T>(w); } };

    _scenes.push_back(metadata);
  }

private:
  std::vector<SceneMetadata> _scenes;
  BaseScene *_current_scene;
  int _current_item = -1;
};

} // namespace sandbox

#endif // __EXAMPLES_MENU_H__
