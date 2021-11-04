#ifndef __DIRECTIONAL_SHADOW_H__
#define __DIRECTIONAL_SHADOW_H__

#include "Render/FrameBuffer.h"
#include "Render/RenderBuffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

namespace engine {

class DirectionalShadow {
public:
  struct Options {
      int width = 10000;
      int height = 10000;
  };

  DirectionalShadow(int width = 1024, int height = 1024);

  glm::mat4 getLigthMatrix(const glm::vec3& direction);

  void prepare();
  void complete();

  uint32_t getShadowMapId();
private:
  Options options;

  std::unique_ptr<Renderer::FrameBuffer>  _frame_buffer;
};

} // namespace engine

#endif // __DIRECTIONAL_SHADOW_H__
