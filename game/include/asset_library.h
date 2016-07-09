#ifndef __ASSET_LIBRARY_H__
#define __ASSET_LIBRARY_H__

#include <vector>
#include <unordered_map>
#include <string>
#include <sys/stat.h>

#include "shader.h"
#include "texture.h"
#include "timer.h"

namespace Game
{
  enum GameAssetType
  {
    SHADER_ASSET,
    TEXTURE_ASSET,
    MESH_ASSET,  
    AUDIO_ASSET
  };

  class GameAsset
  {
  public:
    GameAsset(GameAssetType t) { type = t; file_mod_time = 666; }
    ~GameAsset() {}

    virtual void reload_from_disk() = 0;

    GameAssetType    type;

    //for file monitoring
    std::string      fname;
    uint32_t         file_mod_time;
  };

  class ShaderAsset : public GameAsset
  {
  public:
    ShaderAsset() : GameAsset(SHADER_ASSET) {}
    ~ShaderAsset() {}

    virtual void reload_from_disk();
  
    Graphics::Shader *s;
  };
  
  class TextureAsset : public GameAsset
  {
  public:
    TextureAsset() : GameAsset(TEXTURE_ASSET) {}
    ~TextureAsset() {}

    virtual void reload_from_disk();

    Graphics::Texture2D *t;
  };
  
  class AssetLibrary
  {
  protected:
    std::unordered_map<uint32_t, GameAsset *> lib;
    UI::Timer file_monitor_timer;
    float file_monitor_interval_s;
  public:
    AssetLibrary() { file_monitor_interval_s = 1.0f; }
    ~AssetLibrary() {}

    void add_asset(uint32_t hash_id, GameAsset *a) { lib.insert({ hash_id, a }); }
    GameAsset *retrieve_asset(std::string name);
    Graphics::Shader *retrieve_shader(std::string name);
    Graphics::Texture2D *retrieve_texture(std::string name);

    void init();
    void simulate(const double game_time, const double frame_time);
  };
}

#endif //__ASSET_LIBRARY_H__
