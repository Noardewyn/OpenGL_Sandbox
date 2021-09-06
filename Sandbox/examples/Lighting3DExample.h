#ifndef __LIGHTING3D_EXAMPLE_H__
#define __LIGHTING3D_EXAMPLE_H__

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

class Lighting3DExample : public engine::BaseScene {
public:
  Lighting3DExample(Renderer::Window* window);
  virtual ~Lighting3DExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  std::string _texture_path = assetsPath() + "gold.jpg";
  Renderer::Transform cube_transform;

  char _text_input_buf[128];

  float _light_speed = 1.0f;
  float _light_radius = 2.0f;

  bool _textured_cubes = false;

  std::vector<Renderer::Transform> _cubes;
  std::vector<std::pair<Renderer::Transform, Renderer::Color>> _lights;

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::VertexBufferLayout> _layout;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _shader_light;
  std::unique_ptr<Renderer::Texture> _texture;
  std::unique_ptr<Renderer::Camera> _camera;
};

} // namespace sandbox

#endif // __LIGHTING3D_EXAMPLE_H__
