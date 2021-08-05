#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"

#include "Camera3DExample.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

namespace Sandbox {

  Camera3DExample::Camera3DExample(Renderer::Window* window)
    : BaseScene(window) {
    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    _layout = std::make_unique<Renderer::VertexBufferLayout>();
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    //_ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices));
    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/mvp.frag", "./assets/shaders/mvp.vs");
    _texture = std::make_unique<Renderer::Texture>(_texture_path);
    strcpy(_text_input_buf, _texture_path.c_str());

    Renderer::Transform camera_transform;
    camera_transform.position = {0.0f, 0.0f, -6.0f};
    camera_transform.rotation = {90.0f, 0.0f, 0.0f};

    _camera = std::make_unique<Renderer::Camera>(camera_transform);
    _camera->setPerspective((float)window->getWidth() / window->getHeight());

    _cubes.emplace_back( 0.0f, -1.0f,  0.0f);
    _cubes.emplace_back( 0.0f, -1.0f, -2.0f);
    _cubes.emplace_back( 0.0f, -1.0f,  2.0f);
    _cubes.emplace_back(-2.0f, -1.0f,  0.0f);
    _cubes.emplace_back( 2.0f, -1.0f,  0.0f);
  }

  Camera3DExample::~Camera3DExample() {
    
  }

  void Camera3DExample::onUpdate(float delta_time) {

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
    if (window->isCursorEnabled())
      return;

    _camera->rotate(xoffset, yoffset, 0, delta_time);

    double wheel = tools::InputManager::instance()->wheelY();
    _camera->zoom(wheel, delta_time);

    glm::vec2 camera_move_dir(0.0f);

    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_W))
      camera_move_dir.y += 1.0f;
    if(tools::InputManager::instance()->keyPressed(GLFW_KEY_S))
      camera_move_dir.y -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_A))
      camera_move_dir.x -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_D))
      camera_move_dir.x += 1.0f;

    _camera->move(camera_move_dir, delta_time);
  }

  void Camera3DExample::onRender() {
    window->clear(Renderer::Color::Black);

    _texture->bind();
    _shader->bind();
    
    for (const auto &cube : _cubes) {
      glm::mat4 model = cube.toMatrix();
      _shader->setUniformMatrix4f("model", model);

      _camera->setPerspective((float)window->getWidth() / window->getHeight());
      //_camera->setOrtho(window->getWidth(), window->getHeight());

      _shader->setUniformMatrix4f("viewProjection", _camera->getViewProjectionMatrix());

      _vao->bind();
      //_ibo->bind();

      glDrawArrays(GL_TRIANGLES, 0, 36);
      //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
  }

  void Camera3DExample::onImGuiRender() {
    ImGui::Begin("3d camera example");

    if (ImGui::CollapsingHeader("Controls legend")) {
      ImGui::Text("F2    - enable/disable camera movement");
      ImGui::Text("F3    - switch Wireframe mode");
      ImGui::Text("F10   - fullscreen/windowed");
      ImGui::Text("WASD  - move camera");
      ImGui::Text("Mouse - rotate camera");
      ImGui::Text("Wheel - zoom camera");
    }

    ImGui::InputFloat("zoom", &_camera->fov);

    for (int i = 0; i < _cubes.size(); i++) {
      std::string i_str = std::to_string(i);
      if (ImGui::CollapsingHeader(std::string("Cube Transform: " + i_str).c_str())) {
        ImGui::SliderFloat3(std::string("position" + i_str).c_str(), &_cubes[i].position.x, -10, 10);
        ImGui::SliderFloat3(std::string("scale" + i_str).c_str(), &_cubes[i].scale.x, -10, 10);
        ImGui::SliderFloat3(std::string("rotation" + i_str).c_str(), &_cubes[i].rotation.x, 0, 360);
      }
    }

    ImGui::End();
  }

} // namespace Sandbox
