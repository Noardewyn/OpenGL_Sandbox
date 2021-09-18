#ifndef __MATERIAL_ASSET_H__
#define __MATERIAL_ASSET_H__

#include <string>
#include "engine/Material.h"
#include "engine/assets/Asset.h"

namespace engine {

  class MaterialAsset : public Asset {
  public:
    MaterialAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    virtual ~MaterialAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

    engine::Material& get();

  private:
    engine::Material _material;
  };

} // namespace engine

#endif // __MATERIAL_ASSET_H__
