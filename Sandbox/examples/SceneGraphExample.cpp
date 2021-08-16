﻿#include <imgui.h>

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

#include "SceneGraphExample.h"

namespace Sandbox {

  SceneGraphExample::SceneGraphExample(Renderer::Window* window)
    : engine::Scene(window) {

    Renderer::Transform camera_transform;
    camera_transform.position = {1.0f, 2.0f, -4.0f};
    camera_transform.rotation = {110.0f, -30.0f, 0.0f};

    _camera = std::make_unique<Renderer::Camera>(camera_transform);
    setMainCamera(_camera.get());

    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/multi_light.frag", "./assets/shaders/multi_light.vs");
    _shader_white_color = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");

    _texture_diffuse = std::make_unique<Renderer::Texture>("assets/container2.png");
    _texture_specular = std::make_unique<Renderer::Texture>("assets/container2_specular.png");
    _texture_emission = std::make_unique<Renderer::Texture>("assets/matrix.jpg");

    _texture_earth = std::make_unique<Renderer::Texture>("assets/earth.jpg");

    _box_material = std::make_unique<engine::Material>("textured box");
    _box_material->texture_diffuse = _texture_diffuse.get();
    _box_material->texture_emission = _texture_emission.get();
    _box_material->texture_specular = _texture_specular.get();
    //_box_material->color = Renderer::Color::White;

    _earth_material = std::make_unique<engine::Material>("earth material");
    _earth_material->texture_diffuse = _texture_earth.get();

    _light_source_material = std::make_unique<engine::Material>("light source");
    _light_source_material->color = {1.0, 1.0, 1.0, 1.0};

    _obj_model = std::make_unique<engine::Model>("assets/models/link/pose.obj");
    _cube_mesh = engine::generateCubeMesh();
    _sphere_mesh = engine::generateSphereMesh(16);

    // Entities
    engine::Entity& model_entity = createEntity("3d Model");
    model_entity.transform.position = { 0.0, 0.0, 0.0 };
    model_entity.transform.scale = { 0.2, 0.2, 0.2 };
     
    engine::MeshRenderer* mesh_renderer = model_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _obj_model.get();
    mesh_renderer->shader = _shader.get();

    engine::Entity& cube1_entity = createEntity("Floor");
    cube1_entity.transform.position = {0.0, 0.0, 0.0};
    cube1_entity.transform.scale = { 100.0, 0.2, 100.0 };
    
    mesh_renderer = cube1_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _cube_mesh.get();
    mesh_renderer->material = _box_material.get();
    mesh_renderer->shader = _shader.get();


    //engine::Entity& cube1_entity = createEntity("Earth");
    //cube1_entity.transform.position = {0.0, 0.0, 0.0};
    //cube1_entity.transform.scale = { 1.0, -1.0, 1.0 };

    //engine::MeshRenderer* mesh_renderer = cube1_entity.addComponent<engine::MeshRenderer>();
    //mesh_renderer->target = _sphere_mesh.get();
    //mesh_renderer->material = _earth_material.get();
    //mesh_renderer->shader = _shader.get();

    //addPointLightEntity("Point light 1", { 1.0, 1.0, 0.0 }, Renderer::Color::White);
    addPointLightEntity("Point light 2", { 1.5, 1.0, -1.5 }, Renderer::Color::White);
    addSpotLightEntity("Spot light 1", { 0.0f, 2.0f, 0.0f } , { -0.2f, -1.0f, -0.3f }, Renderer::Color::Red);

    //addDirLightEntity("Directional light 1", { -0.2f, -1.0f, -0.3f }, Renderer::Color::White);
  }

  void SceneGraphExample::addPointLightEntity(const std::string& name, const glm::vec3& position, const Renderer::Color& color) {
    engine::Entity& light_entity = createEntity(name);
    light_entity.transform.position = position;
    light_entity.transform.scale = { 0.2, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _sphere_mesh.get();
    mesh_renderer->material = _light_source_material.get();
    mesh_renderer->shader = _shader.get();

    engine::Light* light_component = light_entity.addComponent<engine::Light>(engine::Light::LightType::Point);
    light_component->color = color;
  }

  void SceneGraphExample::addDirLightEntity(const std::string& name, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& directional_light_entity = createEntity(name);

    engine::Light* light_component = light_component = directional_light_entity.addComponent<engine::Light>(engine::Light::LightType::Directional);
    light_component->direction = direction;
    light_component->color = color;
  }

  void SceneGraphExample::addSpotLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& spot_light_entity = createEntity(name);
    spot_light_entity.transform.position = position;
    spot_light_entity.transform.scale = { 0.2, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = mesh_renderer = spot_light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _sphere_mesh.get();
    mesh_renderer->material = _light_source_material.get();
    mesh_renderer->shader = _shader.get();

    engine::Light* light_component = light_component = spot_light_entity.addComponent<engine::Light>(engine::Light::LightType::Spot);
    light_component->direction = direction;
    light_component->color = color;
  }

  void SceneGraphExample::onUpdate(float delta_time) {

    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_LEFT_SHIFT))
      _camera->moveSpeed = 15.0f;
    else
      _camera->moveSpeed = 5.0f;

    static double mouse_last_x = window->getCursorPosX();
    static double mouse_last_y = window->getCursorPosY();

    double mouse_pos_x = tools::InputManager::instance()->mousePosX();
    double mouse_pos_y = tools::InputManager::instance()->mousePosY();

    GLfloat xoffset = mouse_pos_x - mouse_last_x;
    GLfloat yoffset = mouse_last_y - mouse_pos_y;

    mouse_last_x = mouse_pos_x;
    mouse_last_y = mouse_pos_y;

    // Don't process camera if cursor is visible, in order to 
    // have possibility to click UI without camera movement
    if (!window->isCursorEnabled()) {
      _camera->rotate(xoffset, yoffset, 0, delta_time);

      double wheel = tools::InputManager::instance()->wheelY();
      _camera->zoom(wheel, delta_time);

      glm::vec2 camera_move_dir(0.0f);

      if (tools::InputManager::instance()->keyPressed(GLFW_KEY_W))
        camera_move_dir.y += 1.0f;
      if (tools::InputManager::instance()->keyPressed(GLFW_KEY_S))
        camera_move_dir.y -= 1.0f;
      if (tools::InputManager::instance()->keyPressed(GLFW_KEY_A))
        camera_move_dir.x -= 1.0f;
      if (tools::InputManager::instance()->keyPressed(GLFW_KEY_D))
        camera_move_dir.x += 1.0f;

      _camera->move(camera_move_dir, delta_time);
    }
   
    Scene::onUpdate(delta_time);
  }

  void SceneGraphExample::onRender() {
    window->clear(_clear_color);
    _camera->setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));

    Scene::onRender();
  }

  void SceneGraphExample::onImGuiRender() {
    ImGui::Begin("Scene graph example");

    if (ImGui::CollapsingHeader("Controls legend")) {
      ImGui::Text("F2    - enable/disable camera movement");
      ImGui::Text("F3    - switch Wireframe mode");
      ImGui::Text("F10   - fullscreen/windowed");
      ImGui::Text("WASD  - move camera");
      ImGui::Text("Mouse - rotate camera");
      ImGui::Text("Wheel - zoom camera");
    }

    ImGui::ColorEdit4("Clear color", &_clear_color.r);

    ImGui::Text("Scene graph");
    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
