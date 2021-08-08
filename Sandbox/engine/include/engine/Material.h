#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Render/Texture.h"

namespace engine {

struct Material {
  Renderer::Texture *texture_diffuse;
  Renderer::Texture *texture_specular;
  Renderer::Texture *texture_emission;
};

} // namespace engine

#endif // __MATERIAL_H__
