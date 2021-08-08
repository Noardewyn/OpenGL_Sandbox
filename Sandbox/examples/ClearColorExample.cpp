#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ClearColorExample.h"

namespace Sandbox {

  ClearColorExample::ClearColorExample(Renderer::Window* window)
    : engine::BaseScene(window), _clearColor(Renderer::Color::Green){

  }

  ClearColorExample::~ClearColorExample() {
    
  }

  void ClearColorExample::onUpdate(float delta_time) {
    
  }

  void ClearColorExample::onRender() {
    window->clear(_clearColor);
  }

  void ClearColorExample::onImGuiRender() {
    ImGui::Begin("ClearColorExample");
    ImGui::ColorEdit3("clear color", &_clearColor.r);
    ImGui::End();
  }

} // namespace Sandbox
