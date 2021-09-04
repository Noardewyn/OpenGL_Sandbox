#ifndef __MESH_RENDERER_COMPONENT_H__
#define __MESH_RENDERER_COMPONENT_H__

#include "engine/Component.h"
#include "engine/Mesh.h"
#include "Render/Shader.h"

namespace engine {

class MeshRenderer : public Component {
public:

  IRenderable*           target;
  Material*              material;
  Renderer::Shader*      shader;

  explicit MeshRenderer(Entity* parent = nullptr);

  virtual void onRender() override;
  virtual void onGuiItemRender() override;
  virtual void onUpdate(float delta_time) override;

  bool& getCalculateLighting() { return _calculate_lighting; }
private:
  bool _calculate_lighting = true;
  bool _alpa_blending = true;
  bool _use_textures = true;
  
  void calculateLighting();
};

} // namespace engine

#endif // __MESH_RENDERER_COMPONENT_H__
