#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/Scene.h"
#include "engine/components/Light.h"

#include "engine/components/MeshRenderer.h"

namespace engine {

  MeshRenderer::MeshRenderer(Entity* parent)
    : Component(parent), material(nullptr), target(nullptr), shader_asset(nullptr) {
    _name = "MeshRenderer";
  }

  void MeshRenderer::calculateLighting() {
    const auto& light_sources = _parent->getScene().getEntitiesWithComponent<Light>();

    Renderer::Shader& shader = shader_asset->getShader();
    int light_index = 0;
    
    for(const auto& entity : light_sources) {
      const auto& light  = entity->getComponent<Light>();

      if(!light->isActive() || !entity->isActive())
        continue;

      std::string point_index_str("lights[");
      point_index_str += std::to_string(light_index);
      point_index_str += "].";

      shader.setUniform1f(point_index_str + "type", (int)light->getType());
            
      shader.setUniformColor(point_index_str + "ambient", light->ambient);
      shader.setUniformColor(point_index_str + "diffuse", light->diffuse);
      shader.setUniformColor(point_index_str + "specular", light->specular);
      shader.setUniform1f(point_index_str + "intensity", light->intensity);

      const auto& view_mat = _parent->getScene().mainCamera().getViewMatrix();
      glm::vec4 view_pos = view_mat * glm::vec4(entity->transform.position, 1.0f);

      glm::mat4 model(1.0);
      model = glm::rotate(model, entity->transform.rotation.x, glm::vec3(-1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, entity->transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model, entity->transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      
      glm::vec4 direction(model * glm::vec4(1.0));
      glm::vec4 view_dir = glm::transpose(glm::inverse(view_mat)) * direction;

      if(light->getType() == Light::LightType::Point) {
        shader.setUniform3f(point_index_str + "position", view_pos.x, view_pos.y, view_pos.z);
        shader.setUniform1f(point_index_str + "constant", light->constant);
        shader.setUniform1f(point_index_str + "linear", light->linear);
        shader.setUniform1f(point_index_str + "quadratic", light->quadratic);
      }
      else if(light->getType() == Light::LightType::Spot) {
        shader.setUniform3f(point_index_str + "position", view_pos.x, view_pos.y, view_pos.z);
        shader.setUniform3f(point_index_str + "direction", view_dir.x, view_dir.y, view_dir.z);
        shader.setUniform1f(point_index_str + "cutOff", glm::cos(glm::radians(light->spot_radius)));
        shader.setUniform1f(point_index_str + "outerCutOff", glm::cos(glm::radians(light->spot_radius - light->spot_smooth)));
              
        shader.setUniform1f(point_index_str + "constant", light->constant);
        shader.setUniform1f(point_index_str + "linear", light->linear);
        shader.setUniform1f(point_index_str + "quadratic", light->quadratic);
      }
      else if (light->getType() == Light::LightType::Directional) {
        shader.setUniform3f(point_index_str + "direction", view_dir.x, view_dir.y, view_dir.z);
      }

      light_index++;
    }

    shader.setUniform1i("lights_count", light_index);
  }

  void MeshRenderer::onRender() {
    if(!isActive() || !target || !shader_asset)
      return;

    if (_alpa_blending) {
      glEnable(GL_BLEND);
    }
    else {
      glDisable(GL_BLEND);
    }

    Renderer::Shader& shader = shader_asset->getShader();

    shader.bind();
    const auto &main_camera = _parent->getScene().mainCamera();
    shader.setUniformMatrix4f("model", _parent->transform.toMatrix());
    shader.setUniformMatrix4f("view", main_camera.getViewMatrix());
    shader.setUniformMatrix4f("projection", main_camera.getProjectionMatrix());

    if(_calculate_lighting)
      calculateLighting();

    shader.setUniform1i("calculate_light", _calculate_lighting);

    if(material) {
      target->draw(shader, *material);
    }
    else {
      const Material mat("Incorrect material");
      target->draw(shader, mat);
    }
  }

  void MeshRenderer::onGuiItemRender() {
    ImGui::Checkbox("Calculate lighting", &_calculate_lighting);
    ImGui::Checkbox("Alpha blending", &_alpa_blending);

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
