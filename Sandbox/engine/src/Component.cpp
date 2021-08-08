#include "engine/Component.h"
#include "engine/Entity.h"

namespace engine {

  Component::Component(Entity* parent)
    : _parent(parent), _name("Component") {
    
  }


} // namespace engine
