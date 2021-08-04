#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"

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
    camera_transform.position = {0.0f, 0.0f, -4.0f};
    _camera = std::make_unique<Renderer::Camera>(camera_transform);
  }

  Camera3DExample::~Camera3DExample() {
    
  }

  void Camera3DExample::onUpdate(float delta_time) {
    
  }

  void Camera3DExample::onRender() {
    window->clear(Renderer::Color::Black);

    _texture->bind();
    _shader->bind();
    
    glm::mat4 model = cube_transform.toMatrix();
    _shader->setUniformMatrix4f("model", model);

    // Projection matrix
    _projection_width = window->getWidth();
    _projection_height = window->getHeight();

    glm::mat4 projection(1.0f);
    projection = glm::perspective(45.0f, _projection_width / _projection_height, 0.1f, 100.0f);

    glm::mat4 viewProjection(1.0f);
    viewProjection = projection * _camera->getViewMatrix();

    _shader->setUniformMatrix4f("viewProjection", viewProjection);

    _vao->bind();
    //_ibo->bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  void Camera3DExample::onImGuiRender() {
    ImGui::Begin("3d camera example");
    ImGui::Text(_texture_path.c_str());
    ImGui::InputText("path", _text_input_buf, IM_ARRAYSIZE(_text_input_buf));
    
    if (ImGui::Button("Load")) {
      _texture = std::make_unique<Renderer::Texture>(_text_input_buf);
    }

    ImGui::Text("Cube Transform");
    ImGui::SliderFloat3("position", &cube_transform.position.x, -10, 10);
    ImGui::SliderFloat3("scale", &cube_transform.scale.x, -100, 100);
    ImGui::SliderFloat3("rotaion", &cube_transform.rotation.x, 0, 359);

    ImGui::End();
  }

} // namespace Sandbox
