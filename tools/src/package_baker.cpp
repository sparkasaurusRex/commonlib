#if defined (_WIN32)
#include <Windows.h>
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
  mxml_node_t *shader_node = NULL;
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
  
  do
  {
    shader_node = mxmlFindElement(start_node, tree, "shader", NULL, NULL, MXML_DESCEND);
    if (shader_node)
    {
      ShaderPackageAsset *shader_asset = new ShaderPackageAsset;
      shader_asset->set_type(PACKAGE_ASSET_SHADER);
      assets.push_back(shader_asset);

      cout << "\tLoading shader \"";
      buffer = mxmlElementGetAttr(shader_node, "name");
      cout << buffer << "\"" << endl;
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

    start_node = shader_node;
  } while (shader_node);

  write_package(output_filename);
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
    std::vector<PackageAsset *> textures;
    for (uint32_t i = 0; i < assets.size(); i++)
    {
      switch (assets[i]->get_type())
      {
      case PACKAGE_ASSET_SHADER:
        shaders.push_back((ShaderPackageAsset *)assets[i]);
        break;
      case PACKAGE_ASSET_TEXTURE:
        textures.push_back(assets[i]);
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

    for (uint32_t i = 0; i < shaders.size(); i++)
    {
      ShaderPackageAsset *s = shaders[i];

      //convert name to hash id
      uint32_t hash_id = Math::hash_value_from_string(s->get_name().c_str());
      cout << "\"" << s->get_name().c_str() << "\"" << " -> " << hash_id << endl;

      uint32_t vs_length = s->vs_source.size();
      uint32_t fs_length = s->fs_source.size();

      fwrite(&hash_id, sizeof(uint32_t), 1, fp);
      fwrite(&vs_length, sizeof(uint32_t), 1, fp);
      fwrite(&fs_length, sizeof(uint32_t), 1, fp);

      //write the actual source
      fwrite(s->vs_source.c_str(), sizeof(char), vs_length, fp);
      fwrite(s->fs_source.c_str(), sizeof(char), fs_length, fp);
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