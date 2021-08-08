#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "FragmentCircleExample.h"

static GLfloat vertices[] = {
     1,  1, 0.0f,
     1, -1, 0.0f,
    -1, -1, 0.0f,
    -1,  1, 0.0f
};

static GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

namespace Sandbox {

  FragmentCircleExample::FragmentCircleExample(Renderer::Window* window)
    : engine::BaseScene(window) {

    _quadColor = Renderer::Color::Magenta;
    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);

    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    _ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));
    _plain_shader = std::make_unique<Renderer::Shader>("./assets/shaders/circle.frag", "./assets/shaders/plain.vs");
  }

  FragmentCircleExample::~FragmentCircleExample() {
    
  }

  void FragmentCircleExample::onUpdate(float delta_time) {
    
  }

  void FragmentCircleExample::onRender() {
    window->clear(Renderer::Color::Blue);

    _plain_shader->bind();
    _plain_shader->setUniform2f("iResolution", window->getWidth(), window->getHeight());
    _plain_shader->setUniform1f("iTime", glfwGetTime());

    _vao->bind();
    _ibo->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void FragmentCircleExample::onImGuiRender() {
  }

} // namespace Sandbox
