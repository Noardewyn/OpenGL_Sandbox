#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Render/Texture.h"
#include "Render/Color.h"

namespace engine {

struct Material {
  Renderer::Texture *texture_diffuse = nullptr;
  Renderer::Texture *texture_specular = nullptr;
  Renderer::Texture *texture_emission = nullptr;

  Renderer::Color color;
  Renderer::Color emission_strength;
  float shininess = 32;
};

} // namespace engine

#endif // __MATERIAL_H__
