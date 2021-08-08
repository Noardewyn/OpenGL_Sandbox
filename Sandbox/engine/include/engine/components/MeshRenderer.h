#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "engine/Component.h"
#include "engine/Mesh.h"
#include "Render/Shader.h"

namespace engine {

class MeshRenderer : public Component {
public:

  Mesh *mesh;
  Renderer::Shader* shader;

  explicit MeshRenderer(Entity* parent = nullptr);

  virtual void onRender() override;
  virtual void onGuiItemRender() override;
  virtual void onUpdate(float delta_time) override;
};

} // namespace engine

#endif // __MESH_RENDERER_H__
