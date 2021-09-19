#ifndef __MESH_ASSET_H__
#define __MESH_ASSET_H__

#include <string>
#include "engine/Mesh.h"
#include "engine/assets/Asset.h"

namespace engine {

  class MeshAsset : public Asset {
  public:
    MeshAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    virtual ~MeshAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

  private:
    engine::Mesh _mesh;
  };

} // namespace engine

#endif // __MESH_ASSET_H__
