#ifndef __TEXTURE_EXAMPLE_H__
#define __TEXTURE_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"

#include "BaseScene.h"

namespace Sandbox {

class TextureExample : public BaseScene {
public:
  TextureExample(Renderer::Window* window);
  virtual ~TextureExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  std::string _texture_path = "assets/box.jpg";

  char _text_input_buf[128];

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::VertexBufferLayout> _layout;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Texture> _texture;
};

} // namespace sandbox

#endif // __TEXTURE_EXAMPLE_H__
