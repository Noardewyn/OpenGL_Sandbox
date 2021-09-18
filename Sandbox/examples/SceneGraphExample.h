﻿#ifndef __SCENE_GRAPH_EXAMPLE_H__
#define __SCENE_GRAPH_EXAMPLE_H__

#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera.h"

#include "engine/Material.h"
#include "engine/Mesh.h"
#include "engine/Model.h"
#include "engine/Scene.h"
#include "engine/assets/ShaderAsset.h"
#include "engine/assets/TextureAsset.h"
#include "engine/assets/MaterialAsset.h"
#include "engine/assets/ModelAsset.h"
#include "engine/assets/AssetsWatcher.h"

namespace Sandbox {

class SceneGraphExample : public engine::Scene {
public:
  SceneGraphExample(Renderer::Window* window);

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

  engine::Entity& addPointLightEntity(const std::string& name, const glm::vec3& position, const Renderer::Color& color);
  engine::Entity& addDirLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color);
  engine::Entity& addSpotLightEntity(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const Renderer::Color& color);

private:
  Renderer::Color _clear_color;
  float           _fog_distance = 0;
      
  engine::AssetsWatcher _watcher;

  std::unique_ptr<Renderer::Camera> _camera;
  engine::ShaderAsset* _shader;
  engine::ShaderAsset* _shader_white_color;

  engine::TextureAsset* _texture_earth;
  engine::TextureAsset* _texture_window;

  engine::MaterialAsset* _box_material;
  engine::MaterialAsset* _box_alpha_material;
  engine::MaterialAsset* _earth_material;
  engine::MaterialAsset* _light_source_material;

  std::unique_ptr<engine::Mesh> _cube_mesh;
  std::unique_ptr<engine::Mesh> _sphere_mesh;

  engine::ModelAsset* _sponza_model;
};

} // namespace sandbox

#endif // __SCENE_GRAPH_EXAMPLE_H__
