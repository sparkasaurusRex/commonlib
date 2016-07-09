#include <assert.h>
#include <iostream>
#include "asset_library.h"
#include "platform.h"

using namespace Game;
using namespace Graphics;

using namespace std;

void AssetLibrary::init()
{
  file_monitor_timer.set(file_monitor_interval_s);
  file_monitor_timer.start();
}

void AssetLibrary::simulate(const double game_time, const double frame_time)
{
  if (file_monitor_timer.has_elapsed())
  {
    CHDIR("../data");

    //char cwd[FILENAME_MAX];
    //GETCWD(cwd, sizeof(cwd));
    //cout << "working dir: " << cwd << endl;

    //cout << "file_monitor ping" << endl;
    file_monitor_timer.set(file_monitor_interval_s);
    file_monitor_timer.start();

    //loop through every asset in the library and see if the file has changed.
    std::unordered_map<uint32_t, GameAsset *>::iterator i;
    for (i = lib.begin(); i != lib.end(); i++)
    {
      //ping the file's last modification time off the HD
      GameAsset *asset = i->second;
      assert(asset);
      //cout << asset->fname.c_str() << endl;

      struct stat attrib;
      stat(asset->fname.c_str(), &attrib);
      //cout << "The file was last modified at " << attrib.st_mtime << endl;

      if (asset->file_mod_time != 666)
      {
        if (asset->file_mod_time != attrib.st_mtime)
        {
          SET_TEXT_COLOR(CONSOLE_COLOR_YELLOW);
          cout << "FILE HAS BEEN MODIFIED!!!" << endl;
          SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
          asset->reload_from_disk();
        }
      }
      asset->file_mod_time = (uint32_t)attrib.st_mtime;
      //compare to the last file mod time we're aware of
    }
  }
}

GameAsset *AssetLibrary::retrieve_asset(std::string name)
{
  uint32_t hash_val = Math::hash_value_from_string(name.c_str());
  auto asset = lib.find(hash_val);
  if (asset != lib.end())
  {
    return asset->second;
  }

  cerr << "AssetLibrary::retrieve_asset(): " << name << " Could not retrieve asset!" << endl;
  return NULL;
}

Shader *AssetLibrary::retrieve_shader(std::string name)
{
  ShaderAsset *shader_asset = (ShaderAsset *)retrieve_asset(name);
  assert(shader_asset);
  assert(shader_asset->type == SHADER_ASSET);
  assert(shader_asset->s);
  return shader_asset->s;
}

Texture2D *AssetLibrary::retrieve_texture(std::string name)
{
  TextureAsset *tex_asset = (TextureAsset *)retrieve_asset(name);
  assert(tex_asset);
  assert(tex_asset->type == TEXTURE_ASSET);
  assert(tex_asset->t);
  return tex_asset->t;
}

void ShaderAsset::reload_from_disk()
{
  cout << "Reloading shader from disk" << endl;
}

void TextureAsset::reload_from_disk()
{
  cout << "Reloading texture from disk..." << endl;
  
  //tear-down the current texture
  t->deinit();
  t->set_filename(fname.c_str());
  t->load();
}