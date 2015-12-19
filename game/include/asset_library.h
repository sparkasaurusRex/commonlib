#ifndef __ASSET_LIBRARY_H__
#define __ASSET_LIBRARY_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

template <class T>;
class AssetLibrary
{
  public:
    AssetLibrary()
    {
      search_path.push_back(std::string("./");
    }

    ~AssetLibrary()
    {

    }

    void add_path(std::string p)
    {
      search_path.push_back(p);
    }

    T *load_asset(std::string fname)
    {
      std::cout<<"Asset Library loading "<<fname<<std::endl;
      T *asset = lib[fname];
      std::vector<std::string>::iterator i;
      for(i = search_path.begin(); (i != search_path.end) && !asset; i++)
      {
        std::cout<<(*i)<<std::endl;

        std::string full_fname = (*i) + fname;
        if(access(fname, F_OK | R_OK) != -1)
        {
          //file exists
          asset = ();
        }
      }
      return asset;
    }
  protected:
    std::vector<std::string>                  search_path;
    std::unordered_map<std::string, T *>      lib;
};

#endif //__ASSET_LIBRARY_H__
