#include <imgui.h>

#include "engine/components/Light.h"

namespace engine {

  Light::Light(Entity* parent, const LightType& type)
    : Component(parent), _light_type(type) {
    _name = "Light";
  }

  void Light::onRender() {

  }

  void Light::onGuiItemRender() {
    ImGui::ColorEdit3("color", &color.r);
    ImGui::InputFloat("range", &range);
    ImGui::InputFloat("intensity", &intensity);
  }

  void Light::onUpdate(float delta_time) {
    
  }

} // namespace engine
