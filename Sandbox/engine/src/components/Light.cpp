#include <imgui.h>
#include "engine/Entity.h"
#include "engine/components/MeshRenderer.h"

#include "engine/components/Light.h"
#include "engine/assets/AssetManager.h"

#include <fmt/format.h>

namespace engine {

  Light::Light(Entity* parent, const LightType& type)
    : Component(parent), _light_type(type) {
    _name = "Light";

    if(type == LightType::Directional) {
      _directional_shadow = std::make_unique<DirectionalShadow>(10000, 10000);
    }
  }

  void Light::onRender() {
    if(!isActive()) return;

    auto meshRenderer = _parent->getComponent<MeshRenderer>();

    if(meshRenderer) {
      meshRenderer->setCalculateLighting(false);
    }
  }

  void Light::onGuiItemRender() {

    std::string light_type_str;

    switch (_light_type) {
      case LightType::Directional:
        light_type_str = "Directional";
        break;
      case LightType::Point:
        light_type_str = "Point";
        break;
      case LightType::Spot:
        light_type_str = "Spot";
        break;
      default:
        light_type_str = "Unknown";
    }
    ImGui::Text("Type: '%s'", light_type_str.c_str());
    ImGui::Checkbox("has shadow", &has_shadow);

    if(has_shadow) {
      if (ImGui::CollapsingHeader("Shadow map")) {
        ImGui::Text(fmt::format("{}", _directional_shadow->getShadowMapId()).c_str());
        ImGui::Image((void*)(intptr_t)_directional_shadow->getShadowMapId(), ImVec2(200, 200));
      }
    }

    ImGui::ColorEdit3("color", &color.r);
    ImGui::ColorEdit3("ambient", &ambient.r);
    ImGui::ColorEdit3("diffuse", &diffuse.r);
    ImGui::ColorEdit3("specular", &specular.r);

    ImGui::InputFloat("constant", &constant);
    ImGui::InputFloat("quadratic", &quadratic);
    ImGui::InputFloat("linear", &linear);


    ImGui::SliderFloat3("direction", &direction.x, -1.0, 1.0);
    ImGui::InputFloat("spot radius", &spot_radius);
    ImGui::InputFloat("spot smooth", &spot_smooth);

    ImGui::InputFloat("range", &range);

    ImGui::InputFloat("intensity", &intensity);

    ImGui::Text("Light percents");
    ImGui::SliderFloat("ambient_percent", &ambient_percent, 0.0, 10.0);
    ImGui::SliderFloat("diffuse_percent", &diffuse_percent, 0.0, 10.0);
    ImGui::SliderFloat("specular_percent", &specular_percent, 0.0, 10.0);
  }

  void Light::onUpdate(float delta_time) {
    ambient = { color.r * ambient_percent, color.g * ambient_percent, color.b * ambient_percent };
    diffuse = { color.r * diffuse_percent, color.g * diffuse_percent, color.b * diffuse_percent };
    specular = { color.r * specular_percent, color.g * specular_percent, color.b * specular_percent };
  }

  engine::DirectionalShadow& Light::getDirectionalShadow()
  {
    return *_directional_shadow;
  }

} // namespace engine
