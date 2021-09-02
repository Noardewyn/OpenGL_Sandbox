#ifndef __SCENE_GRAPH_EXAMPLE_H__
#define __SCENE_GRAPH_EXAMPLE_H__

#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "engine/Material.h"
#include "engine/Mesh.h"
#include "engine/Model.h"
#include "engine/Scene.h"

namespace Sandbox {

class SceneGraphExample : public engine::Scene {
public:
  SceneGraphExample(Renderer::Window* window);

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

  engine::Entity& addPointLightEntity(const std::string& name, const glm::vec3& position, const Renderer::Color& color);
  engine::Entity& addDirLightEntity(const std::string& name, const glm::vec3& direction, const Renderer::Color& color);
  engine::Entity& addSpotLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color);

private:
  Renderer::Color _clear_color;
  float           _fog_distance = 0;
      
  std::unique_ptr<Renderer::Camera> _camera;
  std::unique_ptr<Renderer::Shader> _shader;
  std::unique_ptr<Renderer::Shader> _shader_white_color;

  std::unique_ptr<engine::Material> _box_material;
  std::unique_ptr<engine::Material> _box_alpha_material;
  std::unique_ptr<engine::Material> _earth_material;

  std::unique_ptr<engine::Material> _light_source_material;
  std::unique_ptr<Renderer::Texture> _texture_diffuse;
  std::unique_ptr<Renderer::Texture> _texture_specular;
  std::unique_ptr<Renderer::Texture> _texture_emission;
  std::unique_ptr<Renderer::Texture> _texture_earth;
  std::unique_ptr<Renderer::Texture> _texture_window;

  std::unique_ptr<engine::Mesh> _cube_mesh;
  std::unique_ptr<engine::Mesh> _sphere_mesh;

  std::unique_ptr<engine::Model> _obj_model;
};

} // namespace sandbox

#endif // __SCENE_GRAPH_EXAMPLE_H__
