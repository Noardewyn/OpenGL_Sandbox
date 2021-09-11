#ifndef __ASSET_H__
#define __ASSET_H__

#include <string>

namespace engine {

  template<typename T>
  class Asset {
  public:

    enum class AssetType {
      Unknown = 0,
      Texture,
      Shader,
      Model
    };

    Asset(const std::string& asset_path, const AssetType& type, bool auto_reload_enabled = false);
    virtual ~Asset() = default;

    AssetType getType() const;
    std::string getPath() const;

    bool is_auto_reload_enabled() const;
    void set_auto_reload_enabled(bool enabled);
    bool is_reloading() const;

    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void reload() = 0;

    T* get_asset() { 
      return dynamic_cast<T*>(this);
    }

  protected:
    bool set_reloading(bool state);

  private:
    std::string _asset_path;
    AssetType   _type;
    bool        _auto_reload_enabled = false;
    bool        _is_reloading = false;
  };

} // namespace engine

#endif // __ASSET_H__
