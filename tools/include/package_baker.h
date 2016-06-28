#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <mxml.h>

#define PACKAGE_FILE_VERSION 1

namespace Tool
{
  enum PackageAssetType
  {
    PACKAGE_ASSET_SHADER,
    PACKAGE_ASSET_TEXTURE,
    PACKAGE_ASSET_MESH
  };

  class PackageAsset
  {
    friend class PackageBaker;
  protected:
    std::string name;
    PackageAssetType type;
  public:
    PackageAsset() {}
    ~PackageAsset() {}

    void set_name(const char *n) { name = n; }
    std::string get_name() const { return name; }

    void set_type(PackageAssetType t) { type = t; }
    PackageAssetType get_type() const { return type; }
  };

  class ShaderPackageAsset : public PackageAsset
  {
  public:
    ShaderPackageAsset() {}
    ~ShaderPackageAsset() {}

    std::string vs_source;
    std::string fs_source;
  };

  class PackageBaker
  {
  private:
    uint32_t file_version;
    std::vector<PackageAsset *> assets;

    void write_package(std::string output_fname);
  public:
    PackageBaker() { file_version = PACKAGE_FILE_VERSION; }
    ~PackageBaker() {}

    void init() {}
    void bake(mxml_node_t *tree, std::string output_filename);
  };
};