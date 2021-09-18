#include "engine/Mesh.h"

namespace engine {
  Material::Material()
    : name("No Name") {

  }

  Material::Material(const std::string& name)
    : name(name) {
    
  }

} // namespace engine
