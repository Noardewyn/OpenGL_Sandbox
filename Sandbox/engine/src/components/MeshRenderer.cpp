#include <imgui.h>

#include "engine/Scene.h"

#include "engine/components/Light.h"
#include "engine/components/MeshRenderer.h"

namespace engine {

  MeshRenderer::MeshRenderer(Entity* parent)
    : Component(parent) {
    _name = "MeshRenderer";
  }

  void MeshRenderer::calculateLighting() {
    const auto& light_sources = _parent->getScene().getEntitiesWithComponent<Light>();

    for(const auto& entity : light_sources) {
      const auto& light  = entity->getComponent<Light>();

      glm::vec3 diffuseColor = glm::vec3(light->color.r, light->color.g, light->color.b) * 0.8f;
      glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f);

      shader->setUniform1f("light.intensity", light->intensity);
      shader->setUniform3f("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
      shader->setUniform3f("light.diffuse", diffuseColor.x, diffuseColor.x, diffuseColor.z);
      shader->setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
      shader->setUniform3f("lightPos", entity->transform.position.x, entity->transform.position.y, entity->transform.position.z);
    }
  }

  void MeshRenderer::onRender() {
    if(!mesh) return;

    shader->bind();
    const auto &main_camera = _parent->getScene().mainCamera();
    shader->setUniformMatrix4f("model", _parent->transform.toMatrix());
    shader->setUniformMatrix4f("view", main_camera.getViewMatrix());
    shader->setUniformMatrix4f("projection", main_camera.getProjectionMatrix());

    shader->setUniform1f("useFillColor", !_use_textures);

    if (!_use_textures) {
      shader->setUniformColor("fillColor", mesh->material->color);
    }

    if(_calculate_lighting)
      calculateLighting();

    mesh->draw(*shader);
  }

  void MeshRenderer::onGuiItemRender() {
    ImGui::Checkbox("Calculate lighting", &_calculate_lighting);
    ImGui::Checkbox("Use textures", &_use_textures);

    if(!_use_textures) {
      ImGui::ColorEdit3("fill color", &mesh->material->color.r);
    }

  }

  void MeshRenderer::onUpdate(float delta_time) {
    
  }

} // namespace engine
