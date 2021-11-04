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
#include "engine/RenderState.h"

#include "SceneGraphExample.h"

namespace Sandbox {

  SceneGraphExample::SceneGraphExample(Renderer::Window* window)
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

    _shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/multi_light.glsl", true);
    _skybox_shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/skybox.glsl", true);
    _shader_white_color = AssetManager::loadAsset<engine::ShaderAsset>("shaders/mvp_plain.glsl", true);
    _postprocess_shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/postprocess.glsl", true);
    _plain_shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/textured.glsl", true);

    _frame_buffer = std::make_unique<Renderer::FrameBuffer>(window->getWidth(), window->getHeight(), _samples);
    _intermediate_frame_buffer = std::make_unique<Renderer::FrameBuffer>(window->getWidth(), window->getHeight(), 0);
    _frame_buffer->bind();
    _render_buffer = std::make_unique<Renderer::RenderBuffer>(window->getWidth(), window->getHeight(), _samples);

    _texture_earth  = AssetManager::loadAsset<engine::TextureAsset>("earth.jpg");
    _texture_window = AssetManager::loadAsset<engine::TextureAsset>("window.png");
    _texture_brick = AssetManager::loadAsset<engine::TextureAsset>("textures/brickwall.jpg");
    _texture_brick_normal = AssetManager::loadAsset<engine::TextureAsset>("textures/brickwall_normal.jpg");

    _skybox_cubemap = AssetManager::getAsset<engine::TextureAsset>("skybox");

    if (!_skybox_cubemap) {
      const std::vector<std::string> cubemap_pathes = {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
      };

      TextureAsset* skybox_cubemap = new TextureAsset("skybox", cubemap_pathes, false);
      _skybox_cubemap = AssetManager::addAsset<TextureAsset>(skybox_cubemap);
    }

    _box_alpha_material = AssetManager::loadAsset<engine::MaterialAsset>("alpha textured box");
    _box_alpha_material->get().texture_diffuse = _texture_window;

    _brick_material = AssetManager::loadAsset<engine::MaterialAsset>("normal map box");
    _brick_material->get().texture_diffuse = _texture_brick;
    _brick_material->get().texture_normal = _texture_brick_normal;
    _brick_material->get().specular_base = Renderer::Color(0.5, 0.5, 0.5);

    _earth_material = AssetManager::loadAsset<engine::MaterialAsset>("earth material");
    _earth_material->get().texture_diffuse = _texture_earth;

    _light_source_material = AssetManager::loadAsset<engine::MaterialAsset>("light source");
    _light_source_material->get().diffuse_base = { 1.0, 1.0, 1.0, 1.0 };

    _sponza_model = AssetManager::loadAsset<engine::ModelAsset>("models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"
    
    _frame_buffer_quad = engine::generateQuadMesh();
    _plane_mesh = engine::generateMatrixMesh(10, 10, 2, 2);
    _cube_mesh = engine::generateCubeMesh();
    _sphere_mesh = engine::generateSphereMesh(16);
    _skybox_mesh = engine::generateSkyBox();

    // Entities
    {
      engine::Entity& entity = createEntity("Sponza");
      entity.transform.position = { 0.0, 0.0, 0.0 };
      entity.transform.scale = { 0.05, 0.05, 0.05 };

      engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _sponza_model;
      mesh_renderer->shader_asset = _shader;
    }

    {
      engine::Entity& entity = createEntity("Alpha cube");
      entity.transform.position = { 0.0, 0.0, 0.0 };
      entity.transform.scale = { 5.0, 5.0, 5.0 };

      engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _cube_mesh.get();
      mesh_renderer->material = _box_alpha_material;
      mesh_renderer->shader_asset = _shader;
    }

    {
      engine::Entity& entity = createEntity("Brick wall");
      entity.transform.position = { 0, 2.0, -5.0 };
      entity.transform.scale = { 1.0, 1.0, 1.0 };
      //entity.transform.rotation = { 0.0, -90.0, 0.0 };

      engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _plane_mesh.get();
      mesh_renderer->material = _brick_material;
      mesh_renderer->shader_asset = _shader;
    }

    {
      engine::Entity& entity = createEntity("Earth");
      entity.transform.position = { 0.0, 5.0, 5.0 };
      entity.transform.scale = { 1.0, -1.0, 1.0 };

      engine::MeshRenderer* mesh_renderer = entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _sphere_mesh.get();
      mesh_renderer->material = _earth_material;
      mesh_renderer->shader_asset = _shader;
    }

    //addPointLightEntity("Point light 1", { 1.0, 1.0, 0.0 }, Renderer::Color::White);
    addSpotLightEntity("Spot light 1", { -56.0f, 26.0f, -1.0f } , { 0.0f, 0.0f, -120.0f }, Renderer::Color(0.8, 0.6, 0.6));
    addPointLightEntity("Point light 2", { 7.5, 10.0, 1.5 }, Renderer::Color::White);
    engine::Entity& directional = addDirLightEntity("Directional light 1", { 0.0f, 10.0f, 0.0f }, { 20.0f, 74.0f, -105 }, Renderer::Color::White);
  }

  engine::Entity& SceneGraphExample::addPointLightEntity(const std::string& name, const glm::vec3& position, const Renderer::Color& color) {
    engine::Entity& light_entity = createEntity(name);
    light_entity.transform.position = position;
    light_entity.transform.scale = { 0.2, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _sphere_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;
    mesh_renderer->setCalculateLighting(false);

    engine::Light* light_component = light_entity.addComponent<engine::Light>(engine::Light::LightType::Point);
    light_component->color = color;

    return light_entity;
  }

  engine::Entity& SceneGraphExample::addDirLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& directional_light_entity = createEntity(name);
    directional_light_entity.transform.rotation = direction;
    directional_light_entity.transform.scale = { 1.0, 0.2, 0.2 };
    directional_light_entity.transform.rotation = direction;

    engine::MeshRenderer* mesh_renderer = mesh_renderer = directional_light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _cube_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;
    mesh_renderer->setCalculateLighting(false);

    engine::Light* light_component = light_component = directional_light_entity.addComponent<engine::Light>(engine::Light::LightType::Directional);
    light_component->color = color;
    light_component->intensity = 1.0;
    light_component->ambient_percent = 0.04;
    light_component->has_shadow = true;

    return directional_light_entity;
  }

  engine::Entity& SceneGraphExample::addSpotLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color) {
    engine::Entity& spot_light_entity = createEntity(name);
    spot_light_entity.transform.position = position;
    spot_light_entity.transform.rotation = direction;
    spot_light_entity.transform.scale = { 1.0, 0.2, 0.2 };

    engine::MeshRenderer* mesh_renderer = mesh_renderer = spot_light_entity.addComponent<engine::MeshRenderer>();
    mesh_renderer->target = _cube_mesh.get();
    mesh_renderer->material = _light_source_material;
    mesh_renderer->shader_asset = _shader;
    mesh_renderer->setCalculateLighting(false);

    engine::Light* light_component = light_component = spot_light_entity.addComponent<engine::Light>(engine::Light::LightType::Spot);
    light_component->color = color;
    light_component->intensity = 5.0f;

    return spot_light_entity;
  }

  void SceneGraphExample::onUpdate(float delta_time) {
    Scene::onUpdate(delta_time);
  }

  void SceneGraphExample::onRender() {
    using namespace engine;
    RenderState::set_render_stage(RenderState::RenderingStage::SHADOW);
   
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    const auto& light_sources = getEntitiesWithComponent<Light>();

    for (const auto& entity : light_sources) {
      const auto& light = entity->getComponent<Light>();

      if (!light->isActive() || !entity->isActive())
        continue;

      if(light->getType() == Light::LightType::Directional && light->has_shadow) {
        engine::DirectionalShadow& shadow = light->getDirectionalShadow();
        auto shadow_shader_asset = AssetManager::loadAsset<engine::ShaderAsset>("shaders/shadow.glsl", true);
        shadow_shader_asset->get().bind();

        const auto& view_mat = mainCamera().getViewMatrix();
        glm::vec4 view_pos = view_mat * glm::vec4(entity->transform.position, 1.0f);

        glm::mat4 model(1.0);
        model = glm::rotate(model, glm::radians(entity->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(entity->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(entity->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec4 direction(model * glm::vec4(1.0, 0.0, 0.0, 0.0));

        shadow_shader_asset->get().setUniformMatrix4f("lightSpaceMatrix", shadow.getLigthMatrix(direction));
        
        shadow.prepare();
        Scene::onRender();
        shadow.complete();
        _shadow_map = shadow.getShadowMapId();
      }
    }

    RenderState::set_render_stage(RenderState::RenderingStage::NORMAL);
    const int aspect_width = int(((float)window->getWidth() / window->getHeight()) * window->getHeight());
    const int aspect_height = int(((float)window->getHeight() / window->getWidth()) * window->getWidth());
    glViewport(0, 0, aspect_width, aspect_height);

    _frame_buffer->resize(window->getWidth(), window->getHeight());
    _frame_buffer->resample(_samples);
    _frame_buffer->bind();

    _render_buffer->resize(window->getWidth(), window->getHeight());
    _render_buffer->resample(_samples);
    _render_buffer->bind();

    _intermediate_frame_buffer->resize(window->getWidth(), window->getHeight());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    glDepthMask(GL_TRUE);

    window->clear(_clear_color);
    mainCamera().setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));

    glDepthMask(GL_FALSE);
    
    _skybox_shader->get().bind();
    _skybox_shader->get().setUniformMatrix4f("view", glm::mat4(glm::mat3(mainCamera().getViewMatrix())));
    _skybox_shader->get().setUniformMatrix4f("projection", mainCamera().getProjectionMatrix());
    _skybox_mesh.get()->draw(_skybox_shader->get(), _skybox_cubemap->get());

    glDepthMask(GL_TRUE);

    _shader->get().bind();
    _shader->get().setUniform1f("fog_distance", _fog_distance);
    _shader->get().unbind();

    Scene::onRender();

    if (_samples) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _frame_buffer->getId());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediate_frame_buffer->getId());
        glBlitFramebuffer(0, 0, (float)window->getWidth(), (float)window->getHeight(), 0, 0, (float)window->getWidth(), (float)window->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    _frame_buffer->unbind();

    if (_postprocessing)
        _postprocess_shader->get().bind();
    else
        _plain_shader->get().bind();

    if (_samples) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _intermediate_frame_buffer->getTexture());
    }
    else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _frame_buffer->getTexture());
    }

    if (_postprocessing)
        _frame_buffer_quad->draw(_postprocess_shader->get());
    else
        _frame_buffer_quad->draw(_plain_shader->get());
  }

  void SceneGraphExample::onImGuiRender() {
    ImGui::Begin("Shadow map");

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage(
        (void*)_shadow_map, ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(), ImGui::GetCursorScreenPos().y + ImGui::GetWindowHeight()), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    ImGui::Begin("Scene graph example");

    if (ImGui::CollapsingHeader("Controls legend")) {
      ImGui::Text("F2    - enable/disable camera movement");
      ImGui::Text("F3    - switch Wireframe mode");
      ImGui::Text("F10   - fullscreen/windowed");
      ImGui::Text("WASD  - move camera");
      ImGui::Text("Mouse - rotate camera");
      ImGui::Text("Wheel - zoom camera");
    }

    ImGui::Checkbox("Postprocessing", &_postprocessing);
    ImGui::InputInt("MSAA", &_samples);

    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
