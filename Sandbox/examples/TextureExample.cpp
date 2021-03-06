#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Render/Renderer.h"

#include "TextureExample.h"

static GLfloat vertices[] = {
   0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
   0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};

static GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

namespace Sandbox {

  TextureExample::TextureExample(Renderer::Window* window)
    : engine::BaseScene(window) {
    _vao = std::make_unique<Renderer::VertexArray>();
    _vbo = std::make_unique<Renderer::VertexBuffer>(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    _vao->addBuffer(*_vbo, layout);
    _vao->unbind();

    _ibo = std::make_unique<Renderer::IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));
    _shader = std::make_unique<Renderer::Shader>(assetsPath() + "shaders/textured.frag", assetsPath() + "shaders/textured.vs");
    _texture = std::make_unique<Renderer::Texture>(_texture_path);
    strcpy(_text_input_buf, _texture_path.c_str());
  }

  TextureExample::~TextureExample() {
    
  }

  void TextureExample::onUpdate(float delta_time) {
    
  }

  void TextureExample::onRender() {
    window->clear(Renderer::Color::Black);

    _texture->bind();
    Renderer::DrawTriangles(*_vao, *_ibo, *_shader);
  }

  void TextureExample::onImGuiRender() {
    ImGui::Begin("ColoredQuadExample");
    ImGui::Text(_texture_path.c_str());
    ImGui::InputText("path", _text_input_buf, IM_ARRAYSIZE(_text_input_buf));
    
    if (ImGui::Button("Load")) {
      _texture = std::make_unique<Renderer::Texture>(_text_input_buf);
    }

    ImGui::End();
  }

} // namespace Sandbox
