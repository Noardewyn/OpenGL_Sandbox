#ifndef __CLEAR_COLOR_H__
#define __CLEAR_COLOR_H__

#include "BaseScene.h"
#include "Render/Color.h"

namespace Sandbox {

class ClearColorExample : public BaseScene {
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

#endif // __COLOR_H__
