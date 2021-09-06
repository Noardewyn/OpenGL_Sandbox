#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ColoredQuadExample.h"

static GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};

static GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

namespace Sandbox {

  ColoredQuadExample::ColoredQuadExample(Renderer::Window* window)
    : engine::BaseScene(window) {

    _quadColor = Renderer::Color::Magenta;
    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);

    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    _ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));
    _plain_shader = std::make_unique<Renderer::Shader>(assetsPath() + "shaders/plain.frag", assetsPath() + "shaders/plain.vs");
  }

  ColoredQuadExample::~ColoredQuadExample() {
    
  }

  void ColoredQuadExample::onUpdate(float delta_time) {
    
  }

  void ColoredQuadExample::onRender() {
    window->clear(Renderer::Color::Blue);

    _plain_shader->bind();
    _plain_shader->setUniform4f("color", _quadColor.r, _quadColor.g, _quadColor.b, _quadColor.a);

    _vao->bind();
    _ibo->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void ColoredQuadExample::onImGuiRender() {
    ImGui::Begin("ColoredQuadExample");
    ImGui::ColorEdit4("Quad color", &_quadColor.r);
    ImGui::End();
  }

} // namespace Sandbox
