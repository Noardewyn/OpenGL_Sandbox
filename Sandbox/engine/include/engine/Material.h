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
  engine::TextureAsset *texture_height   = nullptr;
  engine::TextureAsset *texture_normal   = nullptr;

  Renderer::Color color;
  Renderer::Color emission_strength;
  float shininess = 32;

  Material();
  Material(const std::string& name);
};

} // namespace engine

#endif // __MATERIAL_H__
