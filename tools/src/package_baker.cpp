#if defined (_WIN32)
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <iostream>
#include <assert.h>
#include "tool.h"
#include "package_baker.h"
#include "math_utility.h"
#include "platform.h"

using namespace Math;
using namespace std;
using namespace Tool;

void PackageBaker::bake(mxml_node_t *tree, std::string output_filename)
{
  cout << "Parsing package xml..." << endl;

  //read all the materials
  mxml_node_t *asset_node = NULL;
  mxml_node_t *start_node = tree;
  const char *buffer = NULL;
  
  //parse the working dir
  mxml_node_t *root_dir_node = mxmlFindElement(start_node, tree, "root_dir", NULL, NULL, MXML_DESCEND);
  if (root_dir_node)
  {
    buffer = mxmlGetText(root_dir_node, NULL);
    cout << "package root dir: " << buffer << endl;
  }
  //change to said directory
  CHDIR(buffer);

  //make sure we succeeded
  char cwd[FILENAME_MAX];
  GETCWD(cwd, sizeof(cwd));
  cout << "working dir: " << cwd << endl;

  //get the output filename
  mxml_node_t *outfile_node = mxmlFindElement(start_node, tree, "output_file", NULL, NULL, MXML_DESCEND);
  if (outfile_node)
  {
    buffer = mxmlGetText(outfile_node, NULL);
    cout << "output package filename: " << buffer << endl;
    output_filename = buffer;
  }
  
  //read all the shader assets
  do
  {
    asset_node = mxmlFindElement(start_node, tree, "shader", NULL, NULL, MXML_DESCEND);
    if (asset_node)
    {
      read_shader_file(asset_node);
    }
    start_node = asset_node;
  } while (asset_node);

  start_node = tree;
  do
  {
    asset_node = mxmlFindElement(start_node, tree, "texture", NULL, NULL, MXML_DESCEND);
    if (asset_node)
    {
      read_texture_file(asset_node);
    }
    start_node = asset_node;
  } while (asset_node);

  write_package(output_filename);
}

void PackageBaker::read_shader_file(mxml_node_t *shader_node)
{
  const char *buffer = NULL;
  ShaderPackageAsset *shader_asset = new ShaderPackageAsset;
  shader_asset->set_type(PACKAGE_ASSET_SHADER);
  assets.push_back(shader_asset);

  SET_TEXT_COLOR(CONSOLE_COLOR_LIGHT_CYAN);
  cout << "\tLoading shader \"";

  buffer = mxmlElementGetAttr(shader_node, "name");
  cout << buffer << "\"" << endl;
  SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);

  shader_asset->set_name(buffer);

  mxml_node_t *vs_node = mxmlFindElement(shader_node, shader_node, "vertex_shader", NULL, NULL, MXML_DESCEND);
  if (vs_node)
  {
    buffer = mxmlGetText(vs_node, NULL);
    cout << "\tvs: " << buffer << " ... ";

    FILE *fp = NULL;
    FOPEN(fp, buffer, "r");
    if (fp)
    {
      fseek(fp, 0, SEEK_END);
      int string_size = ftell(fp);
      rewind(fp);

      char *glsl_source = (char *)malloc(sizeof(char) * (string_size + 1));
      memset(glsl_source, 0, string_size + 1);
      fread(glsl_source, sizeof(char), string_size, fp);
      shader_asset->vs_source = glsl_source;
      free(glsl_source);
      fclose(fp);
      //TODO: compile / check for errors?

      SET_TEXT_COLOR(CONSOLE_COLOR_GREEN);
      cout << "OK" << endl;
      SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    }
    else
    {
      SET_TEXT_COLOR(CONSOLE_COLOR_RED);
      cerr << "Could not open file!" << endl;
      SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    }
  }

  mxml_node_t *fs_node = mxmlFindElement(shader_node, shader_node, "fragment_shader", NULL, NULL, MXML_DESCEND);
  if (fs_node)
  {
    buffer = mxmlGetText(fs_node, NULL);
    cout << "\tfs: " << buffer << " ... ";

    FILE *fp = NULL;
    FOPEN(fp, buffer, "r");
    if (fp)
    {
      fseek(fp, 0, SEEK_END);
      int string_size = ftell(fp);
      rewind(fp);

      char *glsl_source = (char *)malloc(sizeof(char) * (string_size + 1));
      memset(glsl_source, 0, string_size + 1);
      fread(glsl_source, sizeof(char), string_size, fp);
      shader_asset->fs_source = glsl_source;
      free(glsl_source);
      fclose(fp);
      //TODO: compile / check for errors?
      SET_TEXT_COLOR(CONSOLE_COLOR_GREEN);
      cout << "OK" << endl;
      SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    }
    else
    {
      SET_TEXT_COLOR(CONSOLE_COLOR_RED);
      cerr << "Could not open file!" << endl;
      SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    }
  }
}

void PackageBaker::read_texture_file(mxml_node_t *texture_node)
{
  const char *buffer = NULL;
  TexturePackageAsset *texture_asset = new TexturePackageAsset;
  texture_asset->set_type(PACKAGE_ASSET_TEXTURE);
  assets.push_back(texture_asset);

  SET_TEXT_COLOR(CONSOLE_COLOR_LIGHT_CYAN);
  cout << "\tLoading texture \"";
  buffer = mxmlElementGetAttr(texture_node, "name");
  cout << buffer << "\"" << endl;
  SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);

  texture_asset->set_name(buffer);

  buffer = mxmlElementGetAttr(texture_node, "format");
  cout << "\t\ttexture format: " << buffer << endl;

  buffer = mxmlGetText(texture_node, NULL);
  cout << "\t\tsource file: " << buffer << " ... ";

  SDL_Surface *image = IMG_Load(buffer);
  if (!image)
  {
    SET_TEXT_COLOR(CONSOLE_COLOR_RED);
    cerr << "PackageBaker::read_texture_file() - " << IMG_GetError() << endl;
    SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    return;
  }

  SET_TEXT_COLOR(CONSOLE_COLOR_GREEN);
  cout << "OK" << endl;
  SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
  
  cout << "\t\twidth: "<< image->w <<endl;
  cout << "\t\theight: " << image->h << endl;
  texture_asset->bpp = image->format->BytesPerPixel;
  texture_asset->width = image->w;
  texture_asset->height = image->h;

  //copy the texture data to the asset object to be written to the package
  texture_asset->tex_data_size = texture_asset->bpp * image->w * image->h;
  texture_asset->tex_data = malloc(texture_asset->tex_data_size);
  memcpy(texture_asset->tex_data, image->pixels, texture_asset->tex_data_size);

  //deallocate the sdl image
  SDL_FreeSurface(image);
}

void PackageBaker::write_package(std::string output_filename)
{
  cout << endl << "Writing game package to " << output_filename.c_str() << endl;
  FILE *fp;
  FOPEN(fp, output_filename.c_str(), "wb");
  if (fp)
  {
    //collect and count each asset type
    std::vector<ShaderPackageAsset *> shaders;
    std::vector<TexturePackageAsset *> textures;
    for (uint32_t i = 0; i < assets.size(); i++)
    {
      switch (assets[i]->get_type())
      {
      case PACKAGE_ASSET_SHADER:
        shaders.push_back((ShaderPackageAsset *)assets[i]);
        break;
      case PACKAGE_ASSET_TEXTURE:
        textures.push_back((TexturePackageAsset *)assets[i]);
        break;
      }
    }
    uint32_t shader_count = shaders.size();
    uint32_t texture_count = textures.size();

    cout << "Packaging " << shader_count << " shaders..." << endl;
    cout << "Packaging " << texture_count << " textures..." << endl;

    //file header
    fwrite(&file_version, sizeof(uint32_t), 1, fp);
    fwrite(&shader_count, sizeof(uint32_t), 1, fp);
    fwrite(&texture_count, sizeof(uint32_t), 1, fp);

    SET_TEXT_COLOR(CONSOLE_COLOR_LIGHT_CYAN);
    cout << "Writing shader packlets..." << endl;
    SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    for (uint32_t i = 0; i < shaders.size(); i++)
    {
      ShaderPackageAsset *s = shaders[i];
      write_shader_packlet(fp, s);
    }

    SET_TEXT_COLOR(CONSOLE_COLOR_LIGHT_CYAN);
    cout << "Writing texture packlets..." << endl;
    SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
    for (uint32_t i = 0; i < textures.size(); i++)
    {
      TexturePackageAsset *t = textures[i];
      write_texture_packlet(fp, t);
    }

    fclose(fp);
  }
  else
  {
    SET_TEXT_COLOR(CONSOLE_COLOR_RED);
    cerr << "Could not open file for writing!!!" << endl;
    SET_TEXT_COLOR(CONSOLE_COLOR_DEFAULT);
  }
}

void PackageBaker::write_shader_packlet(FILE *fp, ShaderPackageAsset *s)
{
  //convert name to hash id
  uint32_t hash_id = Math::hash_value_from_string(s->get_name().c_str());
  cout << "\"" << s->get_name().c_str() << "\"" << " -> " << hash_id << endl;

  uint32_t vs_length = (s->vs_source.size() + 1) * sizeof(char);
  uint32_t fs_length = (s->fs_source.size() + 1) * sizeof(char);

  fwrite(&hash_id, sizeof(uint32_t), 1, fp);
  fwrite(&vs_length, sizeof(uint32_t), 1, fp);
  fwrite(&fs_length, sizeof(uint32_t), 1, fp);

  char null_char = '\0';

  //write the actual source code
  fwrite(s->vs_source.c_str(), sizeof(char), vs_length, fp);
  fwrite(s->fs_source.c_str(), sizeof(char), fs_length, fp);
}

void PackageBaker::write_texture_packlet(FILE *fp, TexturePackageAsset *t)
{
  uint32_t hash_id = Math::hash_value_from_string(t->get_name().c_str());
  cout << "\"" << t->get_name().c_str() << "\"" << " -> " << hash_id << endl;

  cout << "\tbpp: " << t->bpp << endl;
  cout << "\twidth: " << t->width << endl;
  cout << "\theight: " << t->height << endl;
  cout << "\tdata size: " << t->tex_data_size << endl;

  fwrite(&hash_id, sizeof(uint32_t), 1, fp);
  fwrite(&t->bpp, sizeof(uint32_t), 1, fp);
  fwrite(&t->width, sizeof(uint32_t), 1, fp);
  fwrite(&t->height, sizeof(uint32_t), 1, fp);
  fwrite(&(t->tex_data_size), sizeof(uint32_t), 1, fp);
  fwrite(t->tex_data, t->tex_data_size, 1, fp);
}