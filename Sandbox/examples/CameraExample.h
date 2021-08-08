#ifndef __CAMERA_EXAMPLE_H__
#define __CAMERA_EXAMPLE_H__

#include ""
#include "Render/Color.h"

namespace Sandbox {

class ClearColorExample : public engine::BaseScene {
public:
  ClearColorExample(Renderer::Window* window);
  virtual ~ClearColorExample() override;

  virtual void onUpdate(float delta_time) override;
  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  Renderer::Color _clearColor;
};

} // namespace sandbox

#endif // __CAMERA_EXAMPLE_H__
