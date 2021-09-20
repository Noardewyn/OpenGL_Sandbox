#ifndef __I_RENDERABLE_H__
#define __I_RENDERABLE_H__

#include "Render/Shader.h"
#include "engine/Material.h"

namespace engine {
  class IRenderable {
  public:
    virtual void draw(Renderer::Shader& shader, const Material& material) = 0;
    virtual void draw(Renderer::Shader& shader, const Renderer::Texture& cubemap) {};

  };
} // namespace engine

#endif // __I_RENDERABLE_H__
