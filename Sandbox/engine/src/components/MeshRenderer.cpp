#include <imgui.h>

#include "engine/Scene.h"
#include "engine/components/Light.h"

#include "engine/components/MeshRenderer.h"

namespace engine {

  MeshRenderer::MeshRenderer(Entity* parent)
    : Component(parent), material(nullptr), target(nullptr), shader(nullptr) {
    _name = "MeshRenderer";
  }

  void MeshRenderer::calculateLighting() {
    const auto& light_sources = _parent->getScene().getEntitiesWithComponent<Light>();

    int light_index = 0;
    
    for(const auto& entity : light_sources) {
      const auto& light  = entity->getComponent<Light>();

      if(!light->isActive() || !entity->isActive())
        continue;

      std::string point_index_str("lights[");
      point_index_str += std::to_string(light_index);
      point_index_str += "].";


      shader->setUniform1f(point_index_str + "type", (int)light->getType());

      shader->setUniformColor(point_index_str + "ambient", light->ambient);
      shader->setUniformColor(point_index_str + "diffuse", light->diffuse);
      shader->setUniformColor(point_index_str + "specular", light->specular);
      shader->setUniform1f(point_index_str + "intensity", light->intensity);

      if(light->getType() == Light::LightType::Point) {
        shader->setUniform3f(point_index_str + "position", entity->transform.position.x, entity->transform.position.y, entity->transform.position.z);
        shader->setUniform1f(point_index_str + "constant", light->constant);
        shader->setUniform1f(point_index_str + "linear", light->linear);
        shader->setUniform1f(point_index_str + "quadratic", light->quadratic);
      }
      else if(light->getType() == Light::LightType::Spot) {
        shader->setUniform3f(point_index_str + "position", entity->transform.position.x, entity->transform.position.y, entity->transform.position.z);
        shader->setUniform3f(point_index_str + "direction", light->direction.x, light->direction.y, light->direction.z);
        shader->setUniform1f(point_index_str + "cutOff", glm::cos(glm::radians(light->spot_radius)));
        shader->setUniform1f(point_index_str + "outerCutOff", glm::cos(glm::radians(light->spot_radius - light->spot_smooth)));

        shader->setUniform1f(point_index_str + "constant", light->constant);
        shader->setUniform1f(point_index_str + "linear", light->linear);
        shader->setUniform1f(point_index_str + "quadratic", light->quadratic);
      }
      else if (light->getType() == Light::LightType::Directional) {
        shader->setUniform3f(point_index_str + "direction", light->direction.x, light->direction.y, light->direction.z);
      }

      light_index++;
    }

    shader->setUniform1i("lights_count", light_index);
  }

  void MeshRenderer::onRender() {
    if(!isActive() || !target || !shader)
      return;

    shader->bind();
    const auto &main_camera = _parent->getScene().mainCamera();
    shader->setUniformMatrix4f("model", _parent->transform.toMatrix());
    shader->setUniformMatrix4f("view", main_camera.getViewMatrix());
    shader->setUniformMatrix4f("projection", main_camera.getProjectionMatrix());

    if(_calculate_lighting)
      calculateLighting();

    shader->setUniform1i("calculate_light", _calculate_lighting);

    if(material) {
      target->draw(*shader, *material);
    }
    else {
      const Material mat("Incorrect material");
      target->draw(*shader, mat);
    }
  }

  void MeshRenderer::onGuiItemRender() {
    ImGui::Checkbox("Calculate lighting", &_calculate_lighting);

    if(material) {
      ImGui::Text("material name: %u", material->name);

      if (ImGui::TreeNode("Mesh settings")) {
        ImGui::ColorEdit3("fill color", &material->color.r);
        ImGui::Text("diffuse map: %u", material->texture_diffuse);
        ImGui::Text("specular map: %u", material->texture_specular);
        ImGui::Text("emission map: %u", material->texture_emission);
        ImGui::TreePop();
      }
    }
  }

  void MeshRenderer::onUpdate(float delta_time) {
    
  }

} // namespace engine
