#ifndef __TEXTURE_ASSET_H__
#define __TEXTURE_ASSET_H__

#include <string>
#include "Render/Texture.h"
#include "engine/assets/Asset.h"

namespace engine {

  class TextureAsset : public Asset {
  public:
    TextureAsset(const std::string& asset_path, bool auto_reload_enabled = false);
    virtual ~TextureAsset() = default;

    virtual void load() override;
    virtual void unload() override;
    virtual void reload() override;

    Renderer::Texture& get();

  private:
    Renderer::Texture _texture;
  };

} // namespace engine

#endif // __TEXTURE_ASSET_H__
