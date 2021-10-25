#ifndef __TEXTURE_ASSET_H__
#define __TEXTURE_ASSET_H__

#include <string>
#include "Render/Texture.h"
#include "engine/assets/Asset.h"

namespace engine {

  class TextureAsset : public Asset {
  public:
    TextureAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    TextureAsset(const std::string& name, const std::vector<std::string>& cubemap, bool auto_reload_enabled = false);
    virtual ~TextureAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

    Renderer::Texture& get();
    const Renderer::Texture& TextureAsset::get() const;

  private:
    Renderer::Texture _texture;
  };

} // namespace engine

#endif // __TEXTURE_ASSET_H__
