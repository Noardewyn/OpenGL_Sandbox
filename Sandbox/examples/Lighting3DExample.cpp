#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Renderer.h"

#include "Lighting3DExample.h"

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

  Lighting3DExample::Lighting3DExample(Renderer::Window* window)
    : BaseScene(window) {
    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    _layout = std::make_unique<Renderer::VertexBufferLayout>();
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);
    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    //_ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices));
    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_light.frag", "./assets/shaders/mvp_light.vs");
    _shader_light = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");

    _texture = std::make_unique<Renderer::Texture>(_texture_path);
    strcpy(_text_input_buf, _texture_path.c_str());

    Renderer::Transform camera_transform;
    camera_transform.position = {1.0f, 2.0f, -4.0f};
    camera_transform.rotation = {110.0f, -30.0f, 0.0f};

    _camera = std::make_unique<Renderer::Camera>(camera_transform);

    _cubes.emplace_back( 0.0f, 0.0f,  0.0f);
    
    Renderer::Transform light_transform;
    light_transform.position = { 2.0f, 0.0f, -1.0f };
    light_transform.scale = { 0.3f, 0.3f, 0.3f };

    _lights.push_back({light_transform, Renderer::Color::White});

    _ambient = Renderer::Color(0.1, 0.1, 0.1);
  }

  Lighting3DExample::~Lighting3DExample() {
    
  }

  void Lighting3DExample::onUpdate(float delta_time) {

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

  void Lighting3DExample::onRender() {
    window->clear(Renderer::Color::Black);
    
    _camera->setPerspective((float)window->getWidth() / window->getHeight());

    _shader_light->bind();
    _shader_light->setUniformMatrix4f("viewProjection", _camera->getViewProjectionMatrix());

    _lights[0].first.position.x = cos(_light_speed * glfwGetTime()) * _light_radius;
    _lights[0].first.position.y = sin(_light_speed * glfwGetTime()) * _light_radius;

    for (const auto& light : _lights) {
      glm::mat4 model = light.first.toMatrix();
      _shader_light->setUniformMatrix4f("model", model);
      _shader_light->setUniform4f("color", light.second.r, light.second.g, light.second.b, light.second.a);

      Renderer::DrawTriangles(*_vao, *_shader_light);
    }

    _shader->bind();
    _texture->bind();
    _shader->setUniformMatrix4f("viewProjection", _camera->getViewProjectionMatrix());
    _shader->setUniform3f("lightColor", _lights[0].second.r, _lights[0].second.g, _lights[0].second.b);
    _shader->setUniform3f("ambientStrength", _ambient.r, _ambient.g, _ambient.b);
    _shader->setUniform3f("lightPos", _lights[0].first.position.x, _lights[0].first.position.y, _lights[0].first.position.z);
    _shader->setUniform3f("viewPos", _camera->transform.position.x, _camera->transform.position.y, _camera->transform.position.z);

    for (const auto &cube : _cubes) {
      glm::mat4 model = cube.toMatrix();
      _shader->setUniformMatrix4f("model", model);

      Renderer::DrawTriangles(*_vao, *_shader);
    }
  }

  void Lighting3DExample::onImGuiRender() {
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

    ImGui::Text("Objects");

    for (int i = 0; i < _cubes.size(); i++) {
      std::string i_str = std::to_string(i);
      if (ImGui::CollapsingHeader(std::string("Cube Transform " + i_str).c_str())) {
        ImGui::SliderFloat3(std::string("position" + i_str).c_str(), &_cubes[i].position.x, -10, 10);
        ImGui::SliderFloat3(std::string("scale" + i_str).c_str(), &_cubes[i].scale.x, -10, 10);
        ImGui::SliderFloat3(std::string("rotation" + i_str).c_str(), &_cubes[i].rotation.x, 0, 360);
      }
    }

    ImGui::Text("Light sources");
    ImGui::SliderFloat("rotation speed", &_light_speed, -100, 100);
    ImGui::SliderFloat("rotation radius", &_light_radius, 0.5, 10);

    for (int i = 0; i < _lights.size(); i++) {
      std::string i_str = std::to_string(i);
      if (ImGui::CollapsingHeader(std::string("Light " + i_str).c_str())) {
        ImGui::SliderFloat3(std::string("light position" + i_str).c_str(), &_lights[i].first.position.x, -10, 10);
        ImGui::SliderFloat3(std::string("light scale" + i_str).c_str(), &_lights[i].first.scale.x, -10, 10);
        ImGui::SliderFloat3(std::string("light rotation" + i_str).c_str(), &_lights[i].first.rotation.x, 0, 360);
        ImGui::ColorEdit4(std::string("light rotation" + i_str).c_str(), &_lights[i].second.r);
      }
    }
    ImGui::End();
  }

} // namespace Sandbox
