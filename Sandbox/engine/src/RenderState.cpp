
#include "engine/RenderState.h"

namespace engine {
  engine::RenderState::RenderingStage RenderState::current_render_stage()
  {
    return _stage;
  }

  void RenderState::set_render_stage(const RenderingStage& stage)
  {
    _stage = stage;
  }

  RenderState::RenderingStage RenderState::_stage = RenderState::RenderingStage::NORMAL;

} // namespace engine
