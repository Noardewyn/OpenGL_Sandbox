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

#include "FramebufferExample.h"

static GLfloat vertices[] = {
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
};

static GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

namespace Sandbox {

  FramebufferExample::FramebufferExample(Renderer::Window* window)
    : engine::Scene(window) {
    using namespace engine;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    mainCamera().transform.position = { 1.0f, 2.0f, -4.0f };
    mainCamera().transform.rotation = { 110.0f, -30.0f, 0.0f };

    _shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/default.glsl", true);
    _postprocess_shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/postprocess.glsl", true);
    _plain_shader = AssetManager::loadAsset<engine::ShaderAsset>("shaders/textured.glsl", true);

    _sponza_model = AssetManager::loadAsset<engine::ModelAsset>("models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"

    _frame_buffer = std::make_unique<Renderer::FrameBuffer>(window->getWidth(), window->getHeight(), _samples);
    _intermediate_frame_buffer = std::make_unique<Renderer::FrameBuffer>(window->getWidth(), window->getHeight(), 0);
    _frame_buffer->bind();
    _render_buffer = std::make_unique<Renderer::RenderBuffer>(window->getWidth(), window->getHeight(), _samples);

    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    _ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));

    // Entities
    {
      engine::Entity& model_entity = createEntity("Sponza");
      model_entity.transform.position = { 0.0, 0.0, 0.0 };
      model_entity.transform.scale = { 0.05, 0.05, 0.05 };
      engine::MeshRenderer* mesh_renderer = model_entity.addComponent<engine::MeshRenderer>();
      mesh_renderer->target = _sponza_model;
      mesh_renderer->shader_asset = _shader;
      mesh_renderer->getCalculateLighting() = false;
    }
  }

  FramebufferExample::~FramebufferExample() {
    
  }

  void FramebufferExample::onUpdate(float delta_time) {
    Scene::onUpdate(delta_time);
  }

  void FramebufferExample::onRender() {
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
    glShadeModel(GL_SMOOTH);

    window->clear(_clear_color);
    mainCamera().setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));
    
    Scene::onRender();

    if(_samples) {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, _frame_buffer->getId());
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediate_frame_buffer->getId());
      glBlitFramebuffer(0, 0, (float)window->getWidth(), (float)window->getHeight(), 0, 0, (float)window->getWidth(), (float)window->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    _frame_buffer->unbind();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);

    if(_samples)
      glBindTexture(GL_TEXTURE_2D, _intermediate_frame_buffer->getTexture());
    else 
      glBindTexture(GL_TEXTURE_2D, _frame_buffer->getTexture());

    if(_postprocessing)
      Renderer::DrawTriangles(*_vao, *_ibo, _postprocess_shader->get());
    else
      Renderer::DrawTriangles(*_vao, *_ibo, _plain_shader->get());
  }

  void FramebufferExample::onImGuiRender() {
    ImGui::Begin("Frame buffer example");

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
