﻿#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Renderer.h"

#include "engine/Entity.h"
#include "engine/Mesh.h"
#include "engine/components/MeshRenderer.h"
#include "engine/components/Light.h"

#include "SceneGraphExample.h"

static float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

namespace Sandbox {

  SceneGraphExample::SceneGraphExample(Renderer::Window* window)
    : engine::Scene(window) {

    Renderer::Transform camera_transform;
    camera_transform.position = {1.0f, 2.0f, -4.0f};
    camera_transform.rotation = {110.0f, -30.0f, 0.0f};

    _camera = std::make_unique<Renderer::Camera>(camera_transform);

    Renderer::Transform light_transform;
    light_transform.position = { 2.0f, 0.0f, -1.0f };
    light_transform.scale = { 0.3f, 0.3f, 0.3f };

    //_lights.push_back({light_transform, Renderer::Color::White});

    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/multi_light.frag", "./assets/shaders/multi_light.vs");
    _shader_light = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");

    _shader_light->bind();
    _shader_light->setUniform4f("color", 1.0, 0.3, 0.3, 1.0);
    _shader_light->unbind();

    setMainCamera(_camera.get());

    _texture_diffuse = std::make_unique<Renderer::Texture>("assets/container2.png");
    _texture_specular = std::make_unique<Renderer::Texture>("assets/container2_specular.png");
    _texture_emission = std::make_unique<Renderer::Texture>("assets/matrix.jpg");

    Renderer::VertexBuffer vbo(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    _box_material = std::make_unique<engine::Material>();
    _box_material->texture_diffuse = _texture_diffuse.get();
    _box_material->texture_emission = _texture_emission.get();
    _box_material->texture_specular = _texture_specular.get();

    _cube_mesh = std::make_unique<engine::Mesh>(vbo, layout);
    _cube_mesh->material = _box_material.get();

    engine::Entity& cube1_entity = createEntity("Cube 1");
    cube1_entity.transform.position = {0.0, 0.0, 0.0};
    engine::MeshRenderer* mesh_renderer = cube1_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->mesh = _cube_mesh.get();
    mesh_renderer->shader = _shader.get();

    engine::Entity& light_entity1 = createEntity("Point light 1");
    light_entity1.transform.position = { 1.0, 1.0, 0.0 };
    mesh_renderer = light_entity1.addComponent<engine::MeshRenderer>();
    mesh_renderer->mesh = _cube_mesh.get();
    mesh_renderer->shader = _shader_light.get();

    engine::Light* light_component = light_entity1.addComponent<engine::Light>(engine::Light::LightType::Point);

    engine::Entity& light_entity2 = createEntity("Point light 2");
    light_entity2.transform.position = { -1.0, -1.0, 0.0 };
    mesh_renderer = light_entity2.addComponent<engine::MeshRenderer>();
    mesh_renderer->mesh = _cube_mesh.get();
    mesh_renderer->shader = _shader_light.get();

    light_component = light_entity2.addComponent<engine::Light>(engine::Light::LightType::Point);
    light_component->color = Renderer::Color::Red;

    engine::Entity& directional_light_entity1 = createEntity("Directional light 1");
    mesh_renderer = directional_light_entity1.addComponent<engine::MeshRenderer>();
    mesh_renderer->mesh = _cube_mesh.get();
    mesh_renderer->shader = _shader_light.get();

    light_component = directional_light_entity1.addComponent<engine::Light>(engine::Light::LightType::Directional);
    light_component->color = Renderer::Color::Blue;
    light_component->direction = { -0.2f, -1.0f, -0.3f };
  }

  SceneGraphExample::~SceneGraphExample() {
    
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
    window->clear(Renderer::Color::Black);
    _camera->setPerspective((float)window->getWidth() / (float)window->getHeight());

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

    ImGui::Text("Scene graph");
    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
