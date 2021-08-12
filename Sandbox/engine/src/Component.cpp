#include "engine/Entity.h"

#include "engine/Component.h"

namespace engine {

  Component::Component(Entity* parent)
    : _parent(parent), _name("Component"), active(true) {
    
  }


} // namespace engine
