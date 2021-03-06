#include <imgui.h>

#include "Render/Logger.h"

#include "ExamplesMenu.h"

namespace Sandbox {

  ExamplesMenu::ExamplesMenu(Renderer::Window* window)
    : engine::BaseScene(window), _current_scene(this) {

  }

  ExamplesMenu::~ExamplesMenu() {
    if (_current_scene != this)
      delete _current_scene;
  }

  void ExamplesMenu::onUpdate(float delta_time) {
    
  }

  void ExamplesMenu::onRender() {
    window->clear(Renderer::Color::Blue);
  }

  void ExamplesMenu::onImGuiRender() {
    ImGui::Begin("Menu");
    ImGui::BeginChild("Scrolling");
    for (int i = 0; i < _scenes.size(); i++) {  
      if (_current_item == i) {
        ImGui::Text("-->");
        ImGui::SameLine();
      }

      if (ImGui::Button(_scenes[i].name.c_str()) && i != _current_item) {
        startScene(i);
        ImGui::SetNextWindowCollapsed(true);
      }
    }    
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::EndChild();
    ImGui::End();
  }

  void ExamplesMenu::startScene(int index) {
    if(_current_scene != this)
      delete _current_scene;

    _current_scene = _scenes.at(index).build_function(window).release();
    _current_item = index;

    LOG_APP_INFO("Menu: Scene '{}' started", _scenes.at(_current_item).name);
  }
} // namespace Sandbox
