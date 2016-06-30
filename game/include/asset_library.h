#ifndef __ASSET_LIBRARY_H__
#define __ASSET_LIBRARY_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sys/stat.h>

#include "shader.h"
#include "texture.h"

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
    GameAsset(GameAssetType t) { type = t; }
    ~GameAsset() {}

    GameAssetType type;
    //std::string fname; //TODO: monitor this file for change + reload
  };

  class ShaderAsset : public GameAsset
  {
  public:
    ShaderAsset() : GameAsset(SHADER_ASSET) {}
    ~ShaderAsset() {}
  
    Graphics::Shader *s;
  };
  
  /*
  class TextureAsset : public GameAsset
  {
  public:
    TextureAsset() : GameAsset(TEXTURE_ASSET) {}
    ~TextureAsset() {}

    Graphics::Texture *t;
  };
  */
  class AssetLibrary
  {
  protected:
    std::unordered_map<uint32_t, GameAsset *> lib;

  public:

    AssetLibrary()
    {
    }

    ~AssetLibrary()
    {

    }

    void add_asset(uint32_t hash_id, GameAsset *a)
    {
      lib.insert({ hash_id, a });
    }

    GameAsset *retrieve_asset(std::string fname)
    {
      //std::cout<<"Asset Library loading "<<fname<<std::endl;
      uint32_t hash_val = Math::hash_value_from_string(fname.c_str());
      auto asset = lib.find(hash_val);
      if (asset != lib.end())
      {
        return asset->second;
      }

      std::cerr << "AssetLibrary::retrieve_asset(): " << fname << " Could not retrieve asset!" << std::endl;
      return NULL;
    }

    Graphics::Shader *retrieve_shader(std::string fname)
    {
      ShaderAsset *shader_asset = (ShaderAsset *)retrieve_asset(fname);
      assert(shader_asset);
      return shader_asset->s;
    }
  };
}

#endif //__ASSET_LIBRARY_H__
