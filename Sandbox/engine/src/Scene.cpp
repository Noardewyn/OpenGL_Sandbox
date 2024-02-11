#include <imgui.h>

#include "engine/Scene.h"
#include "Render/InputManager.h"

namespace engine {

Scene::Scene(Renderer::Window* window)
  : BaseScene(window) {
  setMainCamera(&_default_camera);
  _watcher.start_watching();
  
}

void Scene::onUpdate(float delta_time) {
  if (tools::InputManager::instance()->keyPressed(GLFW_KEY_LEFT_SHIFT))
    mainCamera().moveSpeed = 15.0f;
  else
    mainCamera().moveSpeed = 5.0f;

  static double mouse_last_x = window->getCursorPosX();
  static double mouse_last_y = window->getCursorPosY();

  double mouse_pos_x = tools::InputManager::instance()->mousePosX();
  double mouse_pos_y = tools::InputManager::instance()->mousePosY();

  GLfloat xoffset = mouse_pos_x - mouse_last_x;
  GLfloat yoffset = mouse_last_y - mouse_pos_y;

  mouse_last_x = mouse_pos_x;
  mouse_last_y = mouse_pos_y;

  // Don't process camera if cursor is visible, in order to 
  // have possibility to click UI without camera movement
  if (!window->isCursorEnabled()) {
    mainCamera().rotate(xoffset, yoffset, 0, delta_time);

    double wheel = tools::InputManager::instance()->wheelY();
    mainCamera().zoom(wheel, delta_time);

    glm::vec2 camera_move_dir(0.0f);

    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_W))
      camera_move_dir.y += 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_S))
      camera_move_dir.y -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_A))
      camera_move_dir.x -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_D))
      camera_move_dir.x += 1.0f;

    mainCamera().move(camera_move_dir, delta_time);
  }

  _watcher.update();

  for (const auto& entity : _entities) {
    entity->onUpdate(delta_time);
  }
}

void Scene::onRender() {
  for (const auto& entity : _entities) {
    entity->onRender();
  }
}

void Scene::onImGuiRender() {
  ImGui::ColorEdit4("Clear color", &_clear_color.r);
  ImGui::InputFloat("Fog distance", &_fog_distance);

  const char* items[] = { "Default", "Normal_TS", "Normal_VS", "NormalTexture", "Tangent", "Bitangent" };
  const char* current_item = items[static_cast<uint32_t>(_debug_display_mode)];

  if (ImGui::BeginCombo("Display mode##combo", current_item))
  {
    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    {
      bool is_selected = (current_item == items[n]);
      if (ImGui::Selectable(items[n], is_selected))
        _debug_display_mode = static_cast<DebugDisplayMode>(n);
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  if (_main_camera) {
    ImGui::Text("Camera");

    if (ImGui::TreeNode("Transform")) {
      ImGui::SliderFloat3("position", &_main_camera->transform.position.x, -100, 100);
      ImGui::SliderFloat3("scale", &_main_camera->transform.scale.x, -10, 10);

      auto clearButton = [](char* title, float& value, float clear_value = 0) {
        if (ImGui::Button(title)) {
          value = clear_value;
        }
      };

      {
        const int width = ImGui::GetWindowWidth() / 5;
        const char* format = "%.2f";
        ImGui::BeginGroup();
        ImGui::LabelText("", "rotation");
        clearButton("X##rx", _main_camera->transform.rotation.x); ImGui::SameLine(0.0, 0.0);
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r1", &_main_camera->transform.rotation.x, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        clearButton("Y##ry", _main_camera->transform.rotation.y); ImGui::SameLine(0.0, 0.0);
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r2", &_main_camera->transform.rotation.y, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        clearButton("Z##rz", _main_camera->transform.rotation.z); ImGui::SameLine(0.0, 0.0);
        ImGui::SetNextItemWidth(width); ImGui::SliderFloat("##r3", &_main_camera->transform.rotation.z, -360, 360, format); ImGui::SameLine(0.0, 5.0);
        ImGui::EndGroup();
      }

      ImGui::TreePop();
    }
  }
  
  ImGui::Text("Scene graph");

  int index = 0;
  for(const auto& entity : _entities) {
    if (ImGui::CollapsingHeader(entity->getName().c_str())) {
      ImGui::PushID(std::to_string(index).c_str());
      entity->onGuiRender();
      ImGui::PopID();
    }
    index++;
  }
}

Entity& Scene::createEntity(const std::string& name) {
  _entities.push_back(std::make_unique<Entity>(this, name));
  return *_entities.back().get();
}

void Scene::addEntity(Entity* entity) {
  _entities.push_back(std::unique_ptr<Entity>(entity));
}

Entity& Scene::findEntity(const std::string& name) {
  auto predicat = [name](std::unique_ptr<Entity>& e) { return e->getName() == name; };
  return *std::find_if(_entities.begin(), _entities.end(), predicat)->get();
}

void Scene::setMainCamera(Renderer::Camera* cam) {
  assert(cam && "Main camera should be not null");
  _main_camera = cam;
}

Renderer::Camera& Scene::mainCamera() {
  assert(_main_camera && "Main camera is nullptr");
  return *_main_camera;
}

} // namespace engine
