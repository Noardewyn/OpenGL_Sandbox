#include <imgui.h>

#include "engine/Entity.h"

namespace engine {
  Entity::Entity(Scene* scene, const std::string& name)
    :_name(name), _scene(scene), _active(true) {
    
  }

  void Entity::onRender() {
    if(!_active) return;

    for (const auto& component : _components) {
      component->onRender();
    }
  }

  void Entity::onGuiRender() {
    ImGui::Indent(10);
    ImGui::Checkbox("Active", &getActive());

    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::TreeNode("Transform")) {
      ImGui::SliderFloat3("position", &transform.position.x, -10, 10);
      ImGui::SliderFloat3("scale", &transform.scale.x, -10, 10);
      ImGui::SliderFloat3("rotation", &transform.rotation.x, 0, 360);
      ImGui::TreePop();
    }

    for (const auto& component : _components) {
      if (ImGui::TreeNode(component->getName().c_str())) {
        ImGui::Checkbox("Active", &component->active);
        component->onGuiItemRender();
        ImGui::TreePop();
      }
    }

    ImGui::Indent(-10);
  }

  void Entity::onUpdate(float delta_time) {
    if(!_active) return;

    for(const auto &component : _components) {
      if(component->active)
        component->onUpdate(delta_time);
    }
  }

} // namespace engine
