#ifndef __ASSET_LIBRARY_H__
#define __ASSET_LIBRARY_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sys/stat.h>

//namespace Game
//{
  template <class T>
  class AssetLibrary
  {
    public:
      AssetLibrary()
      {
        search_path.push_back(std::string("./"));
      }

      ~AssetLibrary()
      {

      }

      void add_data_file_name(std::string fname)
      {
        if(file_exists(fname))
        {
          file_names.push_back(fname);
          return;
        }
        for(unsigned int i = 0; i < search_path.size(); i++)
        {
          std::string full_fname = search_path[i] + fname;
          if(file_exists(full_fname))
          {
            file_names.push_back(full_fname);
            return;
          }
        }
        std::cerr<<"AssetLibrary::add_data_file_name(): File does not exist: "<<fname<<std::endl;
      }

      void add_path(std::string p)
      {
        search_path.push_back(p);
      }

      void load_all_data()
      {
        std::cout<<"AssetLibrary::load_all_data()..."<<std::endl;
        for(unsigned int i = 0; i < file_names.size(); i++)
        {
          std::cout<<"\t"<<file_names[i]<<std::endl;
        }
      }

      T *retrieve_asset(std::string fname)
      {
        std::cout<<"Asset Library loading "<<fname<<std::endl;
        T *asset = lib[fname];
        std::vector<std::string>::iterator i;
        for(i = search_path.begin(); (i != search_path.end()) && !asset; i++)
        {
          std::cout<<(*i)<<std::endl;

          std::string full_fname = (*i) + fname;
          /*if(access(fname, F_OK | R_OK) != -1)
          {
            //file exists
            asset = ();
          }*/
        }
        return asset;
      }
    protected:

      bool file_exists(std::string fname)
      {
        struct stat buffer;
        return (stat (fname.c_str(), &buffer) == 0);
      }

      std::vector<std::string>                           search_path;
      std::vector<std::string>                           file_names;
      std::unordered_map<unsigned long, T *>             lib;
  };
//}

#endif //__ASSET_LIBRARY_H__
