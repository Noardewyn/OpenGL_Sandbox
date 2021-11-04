#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/Scene.h"
#include "engine/components/Light.h"
#include "engine/RenderState.h"

#include "engine/components/MeshRenderer.h"

namespace engine {

  MeshRenderer::MeshRenderer(Entity* parent)
    : Component(parent), material(nullptr), target(nullptr), shader_asset(nullptr) {
    _name = "MeshRenderer";
    _shadow_shader = AssetManager::loadAsset<engine::ShaderAsset>(shadow_shader_path, true);
  }

  void MeshRenderer::calculateLighting() {
    const auto& light_sources = _parent->getScene().getEntitiesWithComponent<Light>();

    Renderer::Shader& shader = shader_asset->get();
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
      model = glm::rotate(model, glm::radians(entity->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, glm::radians(entity->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model, glm::radians(entity->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
      
      glm::vec4 direction(model * glm::vec4(1.0, 0.0, 0.0, 0.0));
      glm::vec4 view_dir = glm::transpose(glm::inverse(view_mat)) * direction;

      if(light->getType() == Light::LightType::Point) {
        shader.setUniform3f(point_index_str + "position", view_pos.x, view_pos.y, view_pos.z);
        shader.setUniform1f(point_index_str + "constant", light->constant);
        shader.setUniform1f(point_index_str + "linear", light->linear);
        shader.setUniform1f(point_index_str + "quadratic", light->quadratic);
        shader.setUniform1f(point_index_str + "range", light->range);
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

      if(light->has_shadow) {
          shader.setUniformMatrix4f("lightSpaceMatrix", light->getDirectionalShadow().getLigthMatrix(direction));
          shader.setUniform1i(point_index_str + "shadowMap", 11);
          glActiveTexture(GL_TEXTURE0 + 11);
          glBindTexture(GL_TEXTURE_2D, light->getDirectionalShadow().getShadowMapId());
          shader.setUniform1i(point_index_str + "hasShadowMap", true);
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

    if (RenderState::current_render_stage() == RenderState::RenderingStage::SHADOW) {
        _shadow_shader->get().bind();
        _shadow_shader->get().setUniformMatrix4f("model", _parent->transform.toMatrix());
        target->draw(_shadow_shader->get());
        _shadow_shader->get().unbind();
        return;
    }

    Renderer::Shader& shader = shader_asset->get();

    shader.bind();
    const auto &main_camera = _parent->getScene().mainCamera();
    shader.setUniformMatrix4f("model", _parent->transform.toMatrix());
    shader.setUniformMatrix4f("view", main_camera.getViewMatrix());
    shader.setUniformMatrix4f("projection", main_camera.getProjectionMatrix());

    if(_calculate_lighting)
      calculateLighting();

    shader.setUniform1i("calculate_light", _calculate_lighting);

    //Debug modes
    bool normal_maps_mode = _parent->getScene().getDebugMode(DebugMode::ShowNormalMaps);
    shader.setUniform1i("debug_show_normal_maps", normal_maps_mode);

    uint32_t debug_display_mode = (uint32_t)_parent->getScene().getDebugDisplayMode();
    shader.setUniform1i("displayMode", debug_display_mode);

    if(material) {
      target->draw(shader, material->get());
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
      ImGui::Text("material name: %u", material->get().name);

      if (ImGui::TreeNode("Mesh settings")) {
        ImGui::InputFloat("shininess", &material->get().shininess);
        ImGui::ColorEdit3("diffuse base", &material->get().diffuse_base.r);
        ImGui::ColorEdit3("specular base", &material->get().specular_base.r);
        ImGui::ColorEdit3("emission base", &material->get().emission_base.r);

        auto texture_block = [](const std::string& name, const TextureAsset* texture_asset) {
            std::string header_name = name.c_str();
            if(!texture_asset || !texture_asset->get().getRenderId()) {
                header_name += " EMPTY";
            }

            if (ImGui::CollapsingHeader(header_name.c_str())) {
                if(texture_asset && texture_asset->get().getRenderId()) {
                    ImGui::Text(texture_asset->get().image_path.c_str());
                    ImGui::Image((void*)(intptr_t)texture_asset->get().getRenderId(), ImVec2(200, 200));
                }
            }
        };

        texture_block("Deffuse map", material->get().texture_diffuse);
        texture_block("Specular map", material->get().texture_specular);
        texture_block("Emission map", material->get().texture_emission);
        texture_block("Normal map", material->get().texture_normal);
        texture_block("Displacement map", material->get().texture_displacement);

        ImGui::TreePop();
      }
    }
  }

  void MeshRenderer::onUpdate(float delta_time) {
    
  }

} // namespace engine
