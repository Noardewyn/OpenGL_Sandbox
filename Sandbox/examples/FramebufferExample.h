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
#include "engine/assets/ShaderAsset.h"
#include "engine/assets/TextureAsset.h"
#include "engine/assets/MaterialAsset.h"
#include "engine/assets/ModelAsset.h"

namespace Sandbox {

class FramebufferExample : public engine::Scene {
public:
  FramebufferExample(Renderer::Window* window);
  virtual ~FramebufferExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  int  _samples = 4;
  bool _postprocessing = true;

  engine::ShaderAsset* _shader;
  engine::ShaderAsset* _postprocess_shader;
  engine::ShaderAsset* _plain_shader;

  engine::ModelAsset* _sponza_model;

  std::unique_ptr<Renderer::FrameBuffer>  _frame_buffer;
  std::unique_ptr<Renderer::FrameBuffer>  _intermediate_frame_buffer;
  std::unique_ptr<Renderer::RenderBuffer> _render_buffer;

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
};

} // namespace sandbox

#endif // __FRAME_BUFFER_EXAMPLE_H__
