#ifndef __STENCIL_EXAMPLE_H__
#define __STENCIL_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"

#include "engine/Scene.h"
#include "engine/Mesh.h"
#include "engine/assets/ShaderAsset.h"
#include "engine/assets/TextureAsset.h"
#include "engine/assets/MaterialAsset.h"
#include "engine/assets/ModelAsset.h"

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

  engine::ShaderAsset* _shader;
  engine::ShaderAsset* _shader_stencil;

  engine::TextureAsset* _texture_diffuse;
  engine::TextureAsset* _texture_specular;
  engine::TextureAsset* _texture_emission;

  engine::MaterialAsset* _box_material;

  engine::ModelAsset* _sponza_model;
};

} // namespace sandbox

#endif // __STENCIL_EXAMPLE_H__
