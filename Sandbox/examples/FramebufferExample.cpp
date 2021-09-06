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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    Renderer::Transform camera_transform;
    camera_transform.position = { 1.0f, 2.0f, -4.0f };
    camera_transform.rotation = { 110.0f, -30.0f, 0.0f };

    _camera = std::make_unique<Renderer::Camera>(camera_transform);
    setMainCamera(_camera.get());

    _shader = std::make_unique<Renderer::Shader>(assetsPath() + "shaders/default.frag", assetsPath() + "shaders/default.vs");
    _postprocess_shader = std::make_unique<Renderer::Shader>(assetsPath() + "shaders/postprocess.frag", assetsPath() + "shaders/postprocess.vs");

    _sponza_model = std::make_unique<engine::Model>(assetsPath() + "models/sponza/sponza.obj"); // assetsPath() + "models/link/pose.obj"

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
      mesh_renderer->target = _sponza_model.get();
      mesh_renderer->shader = _shader.get();
      mesh_renderer->getCalculateLighting() = false;
    }
  }

  FramebufferExample::~FramebufferExample() {
    
  }

  void FramebufferExample::onUpdate(float delta_time) {
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

  void FramebufferExample::onRender() {
    _frame_buffer->bind();
    _render_buffer->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    window->clear(_clear_color);
    _camera->setPerspective((float)window->getWidth() / std::max((float)window->getHeight(), 1.0f));
    
    Scene::onRender();

    _frame_buffer->unbind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    //glViewport(-1.0, -1.0, 1.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _frame_buffer->getTexture());
    Renderer::DrawTriangles(*_vao, *_ibo, *_postprocess_shader);
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

    ImGui::ColorEdit4("Clear color", &_clear_color.r);

    ImGui::Text("Scene graph");
    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
