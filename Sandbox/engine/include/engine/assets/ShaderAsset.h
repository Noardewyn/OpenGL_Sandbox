#ifndef __SHADER_ASSET_H__
#define __SHADER_ASSET_H__

#include <string>

#include "Render/Shader.h"
#include "engine/assets/Asset.h"

namespace engine {

  class ShaderAsset : public Asset {
  public:
    ShaderAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    virtual ~ShaderAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

    Renderer::Shader& get();
  private:
    Renderer::Shader _shader;
  };

} // namespace engine

#endif // __SHADER_ASSET_H__
