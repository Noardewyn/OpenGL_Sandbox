#ifndef __MODEL_ASSET_H__
#define __MODEL_ASSET_H__

#include <string>
#include "engine/Model.h"
#include "engine/assets/Asset.h"

namespace engine {

  class ModelAsset : public Asset, public IRenderable {
  public:
    ModelAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    virtual ~ModelAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

    virtual void draw(Renderer::Shader& shader, const Material& material) override;
    
    engine::Model& get();

  private:
    engine::Model _model;
  };

} // namespace engine

#endif // __MODEL_ASSET_H__
