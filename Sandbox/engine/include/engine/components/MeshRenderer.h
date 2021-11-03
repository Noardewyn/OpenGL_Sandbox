#ifndef __MESH_RENDERER_COMPONENT_H__
#define __MESH_RENDERER_COMPONENT_H__

#include "engine/Component.h"
#include "engine/Mesh.h"
#include "engine/assets/ShaderAsset.h"
#include "engine/assets/MaterialAsset.h"

namespace engine {

class MeshRenderer : public Component {
public:
  const char* shadow_shader_path = "shaders/shadow.glsl";

  IRenderable*           target;
  MaterialAsset*         material;
  ShaderAsset*           shader_asset;

  explicit MeshRenderer(Entity* parent = nullptr);

  virtual void onRender() override;
  virtual void onGuiItemRender() override;
  virtual void onUpdate(float delta_time) override;

  void setCalculateLighting(bool state) { _calculate_lighting = state; }
  bool getCalculateLighting() { return _calculate_lighting; }

private:
  bool    _calculate_lighting = true;
  bool    _alpa_blending = true;
  bool    _use_textures = true;
  
  engine::ShaderAsset* _shadow_shader;

  void calculateLighting();
};

} // namespace engine

#endif // __MESH_RENDERER_COMPONENT_H__
