#ifndef __ASSET_H__
#define __ASSET_H__

#include <string>

namespace engine {

  enum class AssetType {
    Unknown = 0,
    Texture,
    Shader,
    Material,
    Mesh,
    Model
  };

  class Asset {
  public:
    Asset(const std::string& asset_path, const AssetType& type, bool auto_reload_enabled = false);
    virtual ~Asset() = default;

    AssetType getType() const;
    std::string getPath() const;

    bool is_auto_reload_enabled() const;
    void set_auto_reload_enabled(bool enabled);
    bool is_reloading() const;
    void set_reloading(bool state);

    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void reload() = 0;

  private:
    std::string _asset_path;
    AssetType   _type;
    bool        _auto_reload_enabled = false;
    bool        _is_reloading = false;
  };

} // namespace engine

#endif // __ASSET_H__
