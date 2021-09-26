#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "engine/assets/TextureAsset.h"
#include "Render/Color.h"

namespace engine {

struct Material {
  std::string name;
  engine::TextureAsset *texture_diffuse  = nullptr;
  engine::TextureAsset *texture_specular = nullptr;
  engine::TextureAsset *texture_emission = nullptr;
  engine::TextureAsset *texture_normal   = nullptr;
  engine::TextureAsset *texture_displacement = nullptr;

  Renderer::Color diffuse_base;
  Renderer::Color specular_base;
  Renderer::Color emission_base;

  float shininess = 32;

  Material();
  Material(const std::string& name);
};

} // namespace engine

#endif // __MATERIAL_H__
