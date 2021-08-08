#include "engine/components/MeshRenderer.h"
#include "engine/Scene.h"

namespace engine {

  MeshRenderer::MeshRenderer(Entity* parent)
    : Component(parent) {
    _name = "MeshRenderer";
  }

  void MeshRenderer::onRender() {
    if(!mesh) return;

    shader->bind();
    const auto &main_camera = _parent->getScene().mainCamera();
    shader->setUniformMatrix4f("model", _parent->transform.toMatrix());
    shader->setUniformMatrix4f("viewProjection", main_camera.getViewProjectionMatrix());

    //shader->setUniformMatrix4f("view", main_camera.getViewMatrix());
    //shader->setUniformMatrix4f("projection", main_camera.getProjectionMatrix());
    mesh->draw(*shader);

  }

  void MeshRenderer::onGuiItemRender() {
    
  }

  void MeshRenderer::onUpdate(float delta_time) {
    
  }

} // namespace engine
