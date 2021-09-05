#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Renderer.h"
#include "engine/MeshProvider.h"

#include "engine/components/MeshRenderer.h"

#include "StencilExample.h"

namespace Sandbox {

  StencilExample::StencilExample(Renderer::Window* window)
    : engine::Scene(window) {
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    Renderer::Transform camera_transform;
    camera_transform.position = { 1.0f, 2.0f, -4.0f };
    camera_transform.rotation = { 110.0f, -30.0f, 0.0f };

    _camera = std::make_unique<Renderer::Camera>(camera_transform);
    setMainCamera(_camera.get());

    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/multi_light1.frag", "./assets/shaders/multi_light.vs");
    _shader_stencil = std::make_unique<Renderer::Shader>("./assets/shaders/stencil_outline.frag", "./assets/shaders/stencil_outline.vs");
    _shader_light = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");

    _texture_diffuse = std::make_unique<Renderer::Texture>("assets/container2.png");
    _texture_specular = std::make_unique<Renderer::Texture>("assets/container2_specular.png");
    _texture_emission = std::make_unique<Renderer::Texture>("assets/matrix.jpg");

    _box_material = std::make_unique<engine::Material>("textured box");
    _box_material->texture_diffuse = _texture_diffuse.get();
    _box_material->texture_emission = _texture_emission.get();
    _box_material->texture_specular = _texture_specular.get();

    _cube_mesh = engine::generateCubeMesh();

    _obj_model = std::make_unique<engine::Model>("assets/models/sponza/sponza.obj"); // "assets/models/link/pose.obj"

    // Entities
    {
      engine::Entity& model_entity = createEntity("3d Model");
      model_entity.transform.position = { 0.0, 0.0, 0.0 };
      model_entity.transform.scale = { 0.05, 0.05, 0.05 };
      engine::MeshRenderer* mesh_renderer = model_entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _obj_model.get();
      mesh_renderer->shader = _shader.get();
      mesh_renderer->getCalculateLighting() = false;
    }

    {
      engine::Entity* cube_entity = _cubes.emplace_back(std::make_unique<engine::Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 0.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      engine::MeshRenderer* mesh_renderer = cube_entity->addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material.get();
      mesh_renderer->shader = _shader.get();
    }

    {
      engine::Entity* cube_entity = _cubes.emplace_back(std::make_unique<engine::Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 2.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      engine::MeshRenderer* mesh_renderer = cube_entity->addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material.get();
      mesh_renderer->shader = _shader.get();
    }

    {
      engine::Entity* cube_entity = _cubes.emplace_back(std::make_unique<engine::Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 4.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      engine::MeshRenderer* mesh_renderer = cube_entity->addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material.get();
      mesh_renderer->shader = _shader.get();
    }
  }

  StencilExample::~StencilExample() {
    
  }

  void StencilExample::onUpdate(float delta_time) {
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

  void StencilExample::onRender() {
    window->clear(_clear_color);
    _camera->setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Whether stencil test will pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // To update stencil buffer

    glStencilMask(0x00);

    Scene::onRender();

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    for (const auto& cube : _cubes) {
      cube->transform.scale = { 1.0, 1.0, 1.0 };
      auto* mesh_render = cube->getComponent<engine::MeshRenderer>();
      mesh_render->shader = _shader.get();
      cube->onRender();
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    for (const auto& cube : _cubes) {
      float scale = 1.1;
      cube->transform.scale = { scale, scale, scale };
      auto* mesh_render = cube->getComponent<engine::MeshRenderer>();
      mesh_render->shader = _shader_stencil.get();
      cube->onRender();
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
  }

  void StencilExample::onImGuiRender() {
    ImGui::Begin("Stencil buffer example");

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
