#ifndef __STENCIL_EXAMPLE_H__
#define __STENCIL_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "engine/Mesh.h"
#include "engine/Model.h"
#include "engine/Scene.h"

namespace Sandbox {

class StencilExample : public engine::Scene {
public:
  StencilExample(Renderer::Window* window);
  virtual ~StencilExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  Renderer::Color _clear_color;

  std::list<std::unique_ptr<engine::Entity>> _cubes;

  std::unique_ptr<engine::Mesh> _cube_mesh;

  std::unique_ptr<Renderer::Texture> _texture_diffuse;
  std::unique_ptr<Renderer::Texture> _texture_specular;
  std::unique_ptr<Renderer::Texture> _texture_emission;
  std::unique_ptr<engine::Material> _box_material;

  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _shader_stencil;

  std::unique_ptr<Renderer::Shader> _shader_light;
  std::unique_ptr<Renderer::Texture> _texture;
  std::unique_ptr<Renderer::Camera> _camera;

  std::unique_ptr<engine::Model> _obj_model;
};

} // namespace sandbox

#endif // __STENCIL_EXAMPLE_H__
