#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Renderer.h"

#include "engine/Entity.h"
#include "engine/Mesh.h"
#include "engine/components/MeshRenderer.h"

#include "SceneGraphExample.h"

static float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

namespace Sandbox {

  SceneGraphExample::SceneGraphExample(Renderer::Window* window)
    : engine::Scene(window) {

    Renderer::Transform camera_transform;
    camera_transform.position = {1.0f, 2.0f, -4.0f};
    camera_transform.rotation = {110.0f, -30.0f, 0.0f};

    _camera = std::make_unique<Renderer::Camera>(camera_transform);

    Renderer::Transform light_transform;
    light_transform.position = { 2.0f, 0.0f, -1.0f };
    light_transform.scale = { 0.3f, 0.3f, 0.3f };

    //_lights.push_back({light_transform, Renderer::Color::White});

    _shader = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_light2.frag", "./assets/shaders/mvp_light2.vs");
    _shader_light = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");
    _shader_red = std::make_unique<Renderer::Shader>("./assets/shaders/mvp_plain.frag", "./assets/shaders/mvp_plain.vs");

    _shader_red->bind();
    _shader_red->setUniform4f("color", 1.0, 0.3, 0.3, 1.0);
    _shader_red->unbind();

    setMainCamera(_camera.get());

    _texture_diffuse = std::make_unique<Renderer::Texture>("assets/container2.png");
    _texture_specular = std::make_unique<Renderer::Texture>("assets/container2_specular.png");
    _texture_emission = std::make_unique<Renderer::Texture>("assets/matrix.jpg");

    Renderer::VertexBuffer vbo(vertices, sizeof(vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    _box_material = std::make_unique<engine::Material>();
    _box_material->texture_diffuse = _texture_diffuse.get();
    _box_material->texture_emission = _texture_specular.get();
    _box_material->texture_specular = _texture_emission.get();

    _cube_mesh = std::make_unique<engine::Mesh>(vbo, layout);
    _cube_mesh->material = _box_material.get();

    engine::Entity& cube1_entity = createEntity("Cube 1");
    cube1_entity.transform.position = {0.0, 0.0, 0.0};
    engine::MeshRenderer* meshRenderer = cube1_entity.addComponent<engine::MeshRenderer>();
    meshRenderer->mesh = _cube_mesh.get();
    meshRenderer->shader = _shader_red.get();

    //engine::Entity& cube2_entity = createEntity("Cube 2");

  }

  SceneGraphExample::~SceneGraphExample() {
    
  }

  void SceneGraphExample::onUpdate(float delta_time) {
    Scene::onUpdate(delta_time);

    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_LEFT_SHIFT))
      _camera->moveSpeed = 15.0f;
    else
      _camera->moveSpeed = 5.0f;

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
    if (window->isCursorEnabled())
      return;

    _camera->rotate(xoffset, yoffset, 0, delta_time);

    double wheel = tools::InputManager::instance()->wheelY();
    _camera->zoom(wheel, delta_time);

    glm::vec2 camera_move_dir(0.0f);

    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_W))
      camera_move_dir.y += 1.0f;
    if(tools::InputManager::instance()->keyPressed(GLFW_KEY_S))
      camera_move_dir.y -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_A))
      camera_move_dir.x -= 1.0f;
    if (tools::InputManager::instance()->keyPressed(GLFW_KEY_D))
      camera_move_dir.x += 1.0f;

    _camera->move(camera_move_dir, delta_time);
  }

  void SceneGraphExample::onRender() {
    window->clear(Renderer::Color::Black);
    _camera->setPerspective((float)window->getWidth() / (float)window->getHeight());

    Scene::onRender();
    return;

    //_emission_strength.r = abs(sin(glfwGetTime()));
    _emission_strength.g = sin(glfwGetTime());
    //_emission_strength.b = abs(sin(glfwGetTime()));


    _shader_light->bind();
    _shader_light->setUniformMatrix4f("viewProjection", _camera->getViewProjectionMatrix());

    _lights[0].first.position.x = cos(_light_speed * glfwGetTime()) * _light_radius;
    _lights[0].first.position.y = sin(_light_speed * glfwGetTime()) * _light_radius;

    for (const auto& light : _lights) {
      glm::mat4 model = light.first.toMatrix();
      _shader_light->setUniformMatrix4f("model", model);
      _shader_light->setUniform4f("color", light.second.r, light.second.g, light.second.b, light.second.a);

      Renderer::DrawTriangles(*_vao, *_shader_light);
    }

    //_shader->bind();
    //_shader->setUniformMatrix4f("view", _camera->getViewMatrix());
    //_shader->setUniformMatrix4f("projection", _camera->getProjectionMatrix());

    //_shader->setUniform1f("useFillColor", !_textured_cubes);

    //if(!_textured_cubes) {
    //  _shader->setUniform4f("fillColor", 1.0, 1.0, 1.0, 1.0);
    //}

    //glm::vec3 diffuseColor = glm::vec3(_lights[0].second.r, _lights[0].second.g, _lights[0].second.b) * 0.8f;
    //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    //_shader->setUniform3f("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    //_shader->setUniform3f("light.diffuse", diffuseColor.x, diffuseColor.x, diffuseColor.z);
    //_shader->setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
    ////_shader->setUniform3f("light.position", _lights[0].first.position.x, _lights[0].first.position.y, _lights[0].first.position.z);

    //_shader->setUniform1i("material.diffuse", 0);
    //_shader->setUniform1i("material.specular", 1);
    //_shader->setUniform1i("material.emission", 2);
    //_shader->setUniform3f("material.emissionStrength", _emission_strength.r, _emission_strength.g, _emission_strength.b);
    //_shader->setUniform1f("material.shininess", 32);
    //_shader->setUniform3f("lightPos", _lights[0].first.position.x, _lights[0].first.position.y, _lights[0].first.position.z);

    //_texture_diffuse->bind(0);
    //_texture_specular->bind(1);
    //_texture_emission->bind(2);

    //for (const auto &cube : _cubes) {
    //  glm::mat4 model = cube.toMatrix();
    //  _shader->setUniformMatrix4f("model", model);

    //  Renderer::DrawTriangles(*_vao, *_shader);
    //}
  }

  void SceneGraphExample::onImGuiRender() {
    ImGui::Begin("Scene graph example");

    if (ImGui::CollapsingHeader("Controls legend")) {
      ImGui::Text("F2    - enable/disable camera movement");
      ImGui::Text("F3    - switch Wireframe mode");
      ImGui::Text("F10   - fullscreen/windowed");
      ImGui::Text("WASD  - move camera");
      ImGui::Text("Mouse - rotate camera");
      ImGui::Text("Wheel - zoom camera");
    }

    ImGui::Text("Scene graph");
    Scene::onImGuiRender();

    ImGui::End();
  }

} // namespace Sandbox
