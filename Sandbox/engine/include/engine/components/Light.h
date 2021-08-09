﻿#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__

#include <glm/vec3.hpp>

#include "Render/Color.h"

#include "engine/Component.h"

namespace engine {

class Light : public Component {
public:
  enum class LightType {Unknown, Point, Spot, Directional, Ambient};

  Renderer::Color color = {1, 1, 1, 1};
  Renderer::Color ambient;
  Renderer::Color diffuse;
  Renderer::Color specular = {1, 1, 1, 1};

  float range = 10;
  float intensity = 5;
  float specular_percent = 1.0f;
  float diffuse_percent =  0.8f;
  float ambient_percent =  0.2f;

  glm::vec3 direction;

  explicit Light(Entity* parent = nullptr, const LightType &type = LightType::Point);

  virtual void onRender() override;
  virtual void onGuiItemRender() override;
  virtual void onUpdate(float delta_time) override;

  LightType getType() const { return _light_type; }

private:
  LightType _light_type;
};

} // namespace engine

#endif // __LIGHT_COMPONENT_H__
