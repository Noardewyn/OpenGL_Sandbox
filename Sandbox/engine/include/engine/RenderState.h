#ifndef __RENDER_STATE_H__
#define __RENDER_STATE_H__

namespace engine {
  class RenderState {
  public:
    enum class RenderingStage{ NONE, NORMAL, SHADOW };

    static RenderingStage current_render_stage();
    static void set_render_stage(const RenderingStage& stage);

  private:
    static RenderingStage _stage;
  };
} // namespace engine

#endif // __RENDER_STATE_H__
