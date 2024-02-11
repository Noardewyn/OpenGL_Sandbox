#include <imgui.h>

#include "engine/Entity.h"

namespace engine {
  Entity::Entity(Scene* scene, const std::string& name)
    :_parent(nullptr), _name(name), _scene(scene), _active(true) {
    
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

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Transform")) {
      ImGui::SliderFloat3("position", &transform.position.x, -100, 100);
      ImGui::SliderFloat3("scale", &transform.scale.x, -10, 10);

      auto clearButton = [](char* title, float& value, float clear_value = 0) { 
        if (ImGui::Button(title)) {
          value = clear_value;
        }
      };

      {
        const int width = ImGui::GetWindowWidth() / 5;
        const char* format = "%.2f";
        ImGui::BeginGroup();
        ImGui::LabelText("", "rotation");
        clearButton("X##rx", transform.rotation.x); ImGui::SameLine(0.0, 0.0); 
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r1", &transform.rotation.x, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        clearButton("Y##ry", transform.rotation.y); ImGui::SameLine(0.0, 0.0);
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r2", &transform.rotation.y, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        clearButton("Z##rz", transform.rotation.z); ImGui::SameLine(0.0, 0.0);
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r3", &transform.rotation.z, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        ImGui::EndGroup();
      }

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
