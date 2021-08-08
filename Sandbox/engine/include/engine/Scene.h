#ifndef __SCENE_H__
#define __SCENE_H__

#include "Render/Window.h"
#include "Render/Camera.h"
#include "engine/BaseScene.h"
#include "engine/Entity.h"

namespace engine {

class Scene : public BaseScene {
public:

  Scene(Renderer::Window *window);
  virtual ~Scene() = default;

  void onUpdate(float delta_time) override;
  void onRender() override;
  void onImGuiRender() override;

  Entity& createEntity(const std::string& name);
  void addEntity(Entity *entity);
  Entity& findEntity(const std::string& name);

  void setMainCamera(Renderer::Camera* cam);
  Renderer::Camera& mainCamera();

protected:
  std::list<std::unique_ptr<Entity>> _entities;
  Renderer::Camera *_main_camera;
};

} // namespace engine

#endif // __SCENE_H__
