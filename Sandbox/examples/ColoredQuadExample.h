#ifndef __COLORED_QUAD_EXAMPLE_H__
#define __COLORED_QUAD_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"

#include "engine/BaseScene.h"

namespace Sandbox {

class ColoredQuadExample : public engine::BaseScene {
public:
  ColoredQuadExample(Renderer::Window* window);
  virtual ~ColoredQuadExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
  std::unique_ptr<Renderer::Shader> _plain_shader;

  Renderer::Color _quadColor;
};

} // namespace sandbox

#endif // __COLORED_QUAD_EXAMPLE_H__
