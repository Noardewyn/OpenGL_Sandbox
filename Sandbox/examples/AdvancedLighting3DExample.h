#ifndef __ADVANCED_LIGHTING3D_EXAMPLE_H__
#define __ADVANCED_LIGHTING3D_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "BaseScene.h"

namespace Sandbox {

class AdvancedLighting3DExample : public BaseScene {
public:
  AdvancedLighting3DExample(Renderer::Window* window);
  virtual ~AdvancedLighting3DExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  Renderer::Transform cube_transform;
  Renderer::Color _emission_strength;

  float _light_speed = 1.0f;
  float _light_radius = 2.0f;

  bool _textured_cubes = true;

  std::vector<Renderer::Transform> _cubes;
  std::vector<std::pair<Renderer::Transform, Renderer::Color>> _lights;

  std::unique_ptr<Renderer::VertexArray> _vao;
  std::unique_ptr<Renderer::VertexBuffer> _vbo;
  std::unique_ptr<Renderer::VertexBufferLayout> _layout;
  std::unique_ptr<Renderer::IndexBuffer> _ibo;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _shader_light;
  std::unique_ptr<Renderer::Camera> _camera;

  std::unique_ptr<Renderer::Texture> _texture_diffuse;
  std::unique_ptr<Renderer::Texture> _texture_specular;
  std::unique_ptr<Renderer::Texture> _texture_emission;

};

} // namespace sandbox

#endif // __ADVANCED_LIGHTING3D_EXAMPLE_H__
