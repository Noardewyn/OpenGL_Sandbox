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
    : BaseScene(window) {

    _quadColor = Renderer::Color::Magenta;
    vao = std::make_unique<Renderer::VertexArray>();
    vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    layout = std::make_unique<Renderer::VertexBufferLayout>();
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);

    vao->addBuffer(*vbo, layout);
    vao->unbind();

    ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices));
    plain_shader = std::make_unique<Renderer::Shader>("./assets/shaders/plain.frag", "./assets/shaders/plain.vs");
  }

  ColoredQuadExample::~ColoredQuadExample() {
    
  }

  void ColoredQuadExample::onUpdate(float delta_time) {
    
  }

  void ColoredQuadExample::onRender() {
    window->clear(Renderer::Color::Blue);

    plain_shader->bind();
    plain_shader->setUniform4f("color", _quadColor.r, _quadColor.g, _quadColor.b, _quadColor.a);

    vao->bind();
    ibo->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void ColoredQuadExample::onImGuiRender() {
    ImGui::Begin("ColoredQuadExample");
    ImGui::ColorEdit4("Quad color", &_quadColor.r);
    ImGui::End();
  }

} // namespace Sandbox
