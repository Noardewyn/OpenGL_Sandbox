#ifndef __SCENE_GRAPH_EXAMPLE_H__
#define __SCENE_GRAPH_EXAMPLE_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "engine/Material.h"
#include "engine/Mesh.h"
#include "engine/Scene.h"

namespace Sandbox {

class SceneGraphExample : public engine::Scene {
public:
  SceneGraphExample(Renderer::Window* window);
  virtual ~SceneGraphExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  std::unique_ptr<Renderer::Camera> _camera;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _shader_light;

  std::unique_ptr<engine::Material> _box_material;
  std::unique_ptr<engine::Material> _light_source_material;
  std::unique_ptr<Renderer::Texture> _texture_diffuse;
  std::unique_ptr<Renderer::Texture> _texture_specular;
  std::unique_ptr<Renderer::Texture> _texture_emission;

  std::unique_ptr<engine::Mesh> _cube_mesh;
};

} // namespace sandbox

#endif // __SCENE_GRAPH_EXAMPLE_H__
