#include "engine/Entity.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine {
  Entity::Entity(Scene* scene, const std::string& name)
    :_name(name), _scene(scene) {
    
  }

  void Entity::onRender() {
    for (const auto& component : _components) {
      if (component->active)
        component->onRender();
    }
  }

  void Entity::onGuiRender() {
    ImGui::Text("Components");

    if (ImGui::CollapsingHeader("Transform")) {
      ImGui::SliderFloat3("position", &transform.position.x, -10, 10);
      ImGui::SliderFloat3("scale", &transform.scale.x, -10, 10);
      ImGui::SliderFloat3("rotation", &transform.rotation.x, 0, 360);
    }

    for (const auto& component : _components) {
      if (ImGui::CollapsingHeader(component->getName().c_str())) {
        ImGui::Checkbox("Active", &component->active);
        component->onGuiItemRender();
      }
    }
  }

  void Entity::onUpdate(float delta_time) {
    for(const auto &component : _components) {
      if(component->active)
        component->onUpdate(delta_time);
    }
  }

} // namespace engine
