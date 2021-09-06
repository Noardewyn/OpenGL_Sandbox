#ifndef __CAMERA3D_EXAMPLE_H__
#define __CAMERA3D_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "engine/BaseScene.h"

namespace Sandbox {

class Camera3DExample : public engine::BaseScene {
public:
  Camera3DExample(Renderer::Window* window);
  virtual ~Camera3DExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  std::string _texture_path = assetsPath() + "box.jpg";
  Renderer::Transform cube_transform;

  char _text_input_buf[128];

  std::vector<Renderer::Transform> _cubes;

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::VertexBufferLayout> _layout;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Texture> _texture;
  std::unique_ptr<Renderer::Camera> _camera;
};

} // namespace sandbox

#endif // __CAMERA3D_EXAMPLE_H__
