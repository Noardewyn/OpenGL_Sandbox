#ifndef __FRAME_BUFFER_EXAMPLE_H__
#define __FRAME_BUFFER_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"
#include "Render/FrameBuffer.h"
#include "Render/RenderBuffer.h"

#include "engine/Mesh.h"
#include "engine/Model.h"
#include "engine/Scene.h"

namespace Sandbox {

class FramebufferExample : public engine::Scene {
public:
  FramebufferExample(Renderer::Window* window);
  virtual ~FramebufferExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  Renderer::Color _clear_color;

  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _postprocess_shader;

  std::unique_ptr<Renderer::Camera>  _camera;

  std::unique_ptr<engine::Model> _sponza_model;

  std::unique_ptr<Renderer::FrameBuffer>  _frame_buffer;
  std::unique_ptr<Renderer::RenderBuffer> _render_buffer;

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
};

} // namespace sandbox

#endif // __FRAME_BUFFER_EXAMPLE_H__
