#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/Scene.h"

namespace engine {

Scene::Scene(Renderer::Window* window)
  : BaseScene(window) {
  
}

void Scene::onUpdate(float delta_time) {
  for (const auto& entity : _entities) {
    entity->onUpdate(delta_time);
  }
}

void Scene::onRender() {
  for (const auto& entity : _entities) {
    entity->onRender();
  }
}

void Scene::onImGuiRender() {
  ImGui::Text("Objects");

  for(const auto& entity : _entities) {
    if (ImGui::CollapsingHeader(entity->getName().c_str())) {
      entity->onGuiRender();
    }
  }
}

Entity& Scene::createEntity(const std::string& name) {
  _entities.push_back(std::make_unique<Entity>(this, name));
  return *_entities.back().get();
}

void Scene::addEntity(Entity* entity) {
  _entities.push_back(std::unique_ptr<Entity>(entity));
}

Entity& Scene::findEntity(const std::string& name) {
  auto predicat = [name](std::unique_ptr<Entity>& e) { return e->getName() == name; };
  return *std::find_if(_entities.begin(), _entities.end(), predicat)->get();
}

void Scene::setMainCamera(Renderer::Camera* cam) {
  assert(cam && "Main camera should be not null");
  _main_camera = cam;
}

Renderer::Camera& Scene::mainCamera() {
  assert(_main_camera && "Main camera is nullptr");
  return *_main_camera;
}

} // namespace engine
