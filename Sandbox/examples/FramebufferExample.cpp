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

    _sponza_model = AssetManager::loadAsset<engine::ModelAsset>("models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"

    _frame_buffer = std::make_unique<Renderer::FrameBuffer>(window->getWidth(), window->getHeight());
    _frame_buffer->bind();
    _render_buffer = std::make_unique<Renderer::RenderBuffer>(window->getWidth(), window->getHeight());

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
    _frame_buffer->bind();
    _render_buffer->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    window->clear(_clear_color);
    mainCamera().setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));
    
    Scene::onRender();

    _frame_buffer->unbind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    //glViewport(-1.0, -1.0, 1.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _frame_buffer->getTexture());
    Renderer::DrawTriangles(*_vao, *_ibo, _postprocess_shader->get());
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

    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
