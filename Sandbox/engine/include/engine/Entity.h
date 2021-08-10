#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <memory>
#include <string>
#include <list>

#include "engine/Component.h"
#include "Render/Transform.h"


namespace engine {

// forward declaration of Scene to avoid circular dependency
class Scene;

class Entity {
public:
  Renderer::Transform transform;

  Entity(Scene *scene, const std::string &name = "Entity");

  std::string getName() const { return _name; }
  void setName(const std::string &name) { _name = name; }

  bool& getActive() { return _active; }
  bool isActive() const { return _active; }

  Scene& getScene() { return *_scene; }

  void onRender();
  void onGuiRender();
  void onUpdate(float delta_time);

  void onInit() {}
  void onDelete() {}

  template<typename T, typename... Args>
  T* addComponent(Args... args) {
    assert(!hasComponent<T>() && "Component was already added to the Entity");
    _components.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
    return dynamic_cast<T*>(_components.back().get());
  }

  template<typename T>
  T* getComponent() {
    auto predicate = [](std::unique_ptr<Component>& component)
      { return dynamic_cast<T*>(component.get()) != nullptr; };

    const auto &found_component = std::find_if(_components.begin(), _components.end(), predicate);

    if(found_component == _components.end())
      return nullptr;

    return dynamic_cast<T*>(found_component->get());
  }

  template<typename T>
  bool hasComponent() {
    auto predicate = [](std::unique_ptr<Component>& component)
      { return dynamic_cast<T*>(component.get()) != nullptr; };

    return std::find_if(_components.begin(), _components.end(), predicate) != _components.end();
  }

protected:
  std::string _name;
  bool _active;
  Entity* _parent;
  Scene *_scene;

  std::list<std::unique_ptr<Component>> _components;
};

} // namespace engine

#endif // __ENTITY_H__
