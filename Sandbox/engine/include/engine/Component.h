#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

namespace engine {

// forward declaration of Entity to avoid circular dependency
class Entity;

class Component {
public:
  bool active;

  explicit Component(Entity *parent = nullptr);
  virtual ~Component() = default;

  virtual void onRender() {}
  virtual void onGuiItemRender() {}
  virtual void onUpdate(float delta_time) {}

  virtual void onInit() {}
  virtual void onDelete() {}

  std::string getName() const { return _name; }

  bool isActive() const { return active; }
  void setActive(bool state) { active = state; }

protected:
  Entity     *_parent;
  std::string _name;
};

} // namespace engine

#endif // __COMPONENT_H__
