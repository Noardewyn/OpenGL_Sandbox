#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Renderer.h"

#include "engine/Entity.h"
#include "engine/Mesh.h"
#include "engine/MeshProvider.h"
#include "engine/components/MeshRenderer.h"
#include "engine/components/Light.h"

#include "StressTestExample.h"

namespace Sandbox {

  StressTestExample::StressTestExample(Renderer::Window* window)
    : engine::Scene(window) {
    using namespace engine;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    mainCamera().transform.position = { 1.0f, 2.0f, -4.0f };
    mainCamera().transform.rotation = { 110.0f, -30.0f, 0.0f };

    _shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/default.glsl", true);
    _shader_white_color = AssetManager::loadAsset<engine::ShaderAsset>("shaders/mvp_plain.glsl", true);

    _texture_earth  = AssetManager::loadAsset<engine::TextureAsset>("earth.jpg");

    _earth_material = AssetManager::loadAsset<engine::MaterialAsset>("earth material");
    _earth_material->get().texture_diffuse = _texture_earth;

    _light_source_material = AssetManager::loadAsset<engine::MaterialAsset>("light source");
    _light_source_material->get().diffuse_base = { 1.0, 1.0, 1.0, 1.0 };

    //_sponza_model = AssetManager::loadAsset<engine::ModelAsset>("models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"
    
    _sphere_mesh = engine::generateSphereMesh(16);

    // Entities
    //{
    //  engine::Entity& entity = createEntity("Sponza");
    //  entity.transform.position = { 0.0, 0.0, 0.0 };
    //  entity.transform.scale = { 0.05, 0.05, 0.05 };

    //  engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
    //  mesh_renderer->target = _sponza_model;
    //  mesh_renderer->shader_asset = _shader;
    //}

    const int objects_count = 50;
    const int step = 2;

    for (int i = 0; i < objects_count; i++) {
      for (int j = 0; j < objects_count; j++) {
        {
          engine::Entity& entity = createEntity("Earth");
          entity.transform.position = { i * step, 0.0, j * step };
          entity.transform.scale = { 1.0, -1.0, 1.0 };

          engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
          mesh_renderer->target = _sphere_mesh.get();
          mesh_renderer->material = _earth_material;
          mesh_renderer->shader_asset = _shader;
        }
      }
    }

    //addPointLightEntity("Point light 1", { 1.0, 1.0, 0.0 }, Renderer::Color::White);
    addSpotLightEntity("Spot light 1", { 0.0f, 10.0f, 0.0f } , { 0.0f, 0.0f, -120.0f }, Renderer::Color(0.4, 0.2, 0.2));
    addPointLightEntity("Point light 2", { 7.5, 1.0, 1.5 }, Renderer::Color::White);
    engine::Entity& directional = addDirLightEntity("Directional light 1", { 0.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, -120 }, Renderer::Color::White);
    auto* dir_light = directional.getComponent<engine::Light>();
    dir_light->intensity = 0.1;
  }

  engine::Entity& StressTestExample::addPointLightEntity(const std::string& name, const glm::vec3& position, const Renderer::Color& color) {
    engine::Entity& light_entity = createEntity(name);
    light_entity.transform.position = position;
    light_entity.transform.scale = { 0.2, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _sphere_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;

    engine::Light* light_component = light_entity.addComponent<engine::Light>(engine::Light::LightType::Point);
    light_component->color = color;

    return light_entity;
  }

  engine::Entity& StressTestExample::addDirLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& directional_light_entity = createEntity(name);
    directional_light_entity.transform.rotation = direction;
    directional_light_entity.transform.scale = { 1.0, 0.2, 0.2 };
    directional_light_entity.transform.rotation = direction;

    engine::MeshRenderer* mesh_renderer = mesh_renderer = directional_light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _cube_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;

    engine::Light* light_component = light_component = directional_light_entity.addComponent<engine::Light>(engine::Light::LightType::Directional);
    light_component->direction = direction;
    light_component->color = color;
    light_component->ambient_percent = 0.1;

    return directional_light_entity;
  }

  engine::Entity& StressTestExample::addSpotLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& spot_light_entity = createEntity(name);
    spot_light_entity.transform.position = position;
    spot_light_entity.transform.rotation = direction;
    spot_light_entity.transform.scale = { 1.0, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = mesh_renderer = spot_light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _cube_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;

    engine::Light* light_component = light_component = spot_light_entity.addComponent<engine::Light>(engine::Light::LightType::Spot);
    light_component->direction = direction;
    light_component->color = color;

    return spot_light_entity;
  }

  void StressTestExample::onUpdate(float delta_time) {
    Scene::onUpdate(delta_time);
  }

  void StressTestExample::onRender() {
    window->clear(_clear_color);
    mainCamera().setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));

    _shader->get().bind();
    _shader->get().setUniform1f("fog_distance", _fog_distance);
    _shader->get().unbind();

    Scene::onRender();
  }

  void StressTestExample::onImGuiRender() {
    ImGui::Begin("Scene graph example");

    if (ImGui::CollapsingHeader("Controls legend")) {
      ImGui::Text("F2    - enable/disable camera movement");
      ImGui::Text("F3    - switch Wireframe mode");
      ImGui::Text("F10   - fullscreen/windowed");
      ImGui::Text("WASD  - move camera");
      ImGui::Text("Mouse - rotate camera");
      ImGui::Text("Wheel - zoom camera");
    }

    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
