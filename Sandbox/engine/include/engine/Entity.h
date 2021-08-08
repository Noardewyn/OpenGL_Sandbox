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

  Scene& getScene() { return *_scene; }

  void onRender();
  void onGuiRender();
  void onUpdate(float delta_time);

  void onInit() {}
  void onDelete() {}

  template<typename T>
  T* addComponent() {
    assert(!hasComponent<T>() && "Component was already added to the Entity");
    //return *_components.emplace_back<T>(this);
    _components.push_back(new T(this));
    return dynamic_cast<T*>(_components.back());
  }

  template<typename T>
  T& getComponent() {
    assert(hasComponent<T>() && "Component is not exists on the Entity");
    auto predicate = [](Component* component) { return dynamic_cast<T*>(component) != nullptr; };
    return *std::find_if(_components.begin(), _components.end(), predicate);
  }

  template<typename T>
  bool hasComponent() {
    auto predicate = [](Component* component) { return dynamic_cast<T*>(component) != nullptr; };
    return std::find_if(_components.begin(), _components.end(), predicate) != _components.end();
  }

protected:
  std::string _name;
  Entity* _parent;
  Scene *_scene;

  std::list<Component*> _components;
};

} // namespace engine

#endif // __ENTITY_H__
