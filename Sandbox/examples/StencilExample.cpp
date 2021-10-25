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

#include "engine/assets/AssetManager.h"
#include "engine/components/MeshRenderer.h"

#include "StencilExample.h"

namespace Sandbox {

  StencilExample::StencilExample(Renderer::Window* window)
    : engine::Scene(window) {
    using namespace engine;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    mainCamera().transform.position = { 1.0f, 2.0f, -4.0f };
    mainCamera().transform.rotation = { 110.0f, -30.0f, 0.0f };

    _shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/default.glsl", true);
    _shader_stencil = AssetManager::loadAsset<engine::ShaderAsset>("shaders/stencil_outline.glsl", true);

    _texture_diffuse = AssetManager::loadAsset<engine::TextureAsset>("container2.png");
    _texture_specular = AssetManager::loadAsset<engine::TextureAsset>("container2_specular.png");
    _texture_emission = AssetManager::loadAsset<engine::TextureAsset>("matrix.jpg");

    _box_material = AssetManager::loadAsset<engine::MaterialAsset>("textured box");
    _box_material->get().texture_diffuse = _texture_diffuse;
    _box_material->get().texture_emission = _texture_emission;
    _box_material->get().texture_specular = _texture_specular;

    _sponza_model = AssetManager::loadAsset<engine::ModelAsset>("models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"
    
    _cube_mesh = engine::generateCubeMesh();

    // Entities
    {
      Entity& model_entity = createEntity("3d Model");
      model_entity.transform.position = { 0.0, 0.0, 0.0 };
      model_entity.transform.scale = { 0.05, 0.05, 0.05 };
      MeshRenderer* mesh_renderer = model_entity.addComponent<MeshRenderer>();
      mesh_renderer->target = _sponza_model;
      mesh_renderer->shader_asset = _shader;
    }

    {
      Entity* cube_entity = _cubes.emplace_back(std::make_unique<Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 0.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      MeshRenderer* mesh_renderer = cube_entity->addComponent<MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material;
      mesh_renderer->shader_asset = _shader;
    }

    {
      Entity* cube_entity = _cubes.emplace_back(std::make_unique<Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 2.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      MeshRenderer* mesh_renderer = cube_entity->addComponent<MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material;
      mesh_renderer->shader_asset = _shader;
    }

    {
      Entity* cube_entity = _cubes.emplace_back(std::make_unique<Entity >(this, "cube")).get();
      cube_entity->transform.position = { 0.0f, 10.0f, 4.0f };
      cube_entity->transform.scale = { 1.0, 1.0, 1.0 };

      MeshRenderer* mesh_renderer = cube_entity->addComponent<MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_material;
      mesh_renderer->shader_asset = _shader;
    }
  }

  StencilExample::~StencilExample() {
    
  }

  void StencilExample::onUpdate(float delta_time) {
    Scene::onUpdate(delta_time);
  }

  void StencilExample::onRender() {
    window->clear(_clear_color);
    mainCamera().setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));
    
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
      mesh_render->shader_asset = _shader;
      cube->onRender();
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    for (const auto& cube : _cubes) {
      float scale = 1.1;
      cube->transform.scale = { scale, scale, scale };
      auto* mesh_render = cube->getComponent<engine::MeshRenderer>();
      mesh_render->shader_asset = _shader_stencil;
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

    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
