#include <iostream>
#include <assert.h>
#include "bphys_baker.h"

using namespace std;
using namespace Tool;

void BPhysBaker::bake(FILE *f)
{
  BPhysHeader bph;
  fread(&bph, sizeof(BPhysHeader), 1, f);
  assert(bph.bphys[0] == 'B' && bph.bphys[1] == 'P' && bph.bphys[2] == 'H' && bph.bphys[3] == 'Y');
  assert(bph.bphys[4] == 'S' && bph.bphys[5] == 'I' && bph.bphys[6] == 'C' && bph.bphys[7] == 'S');

  switch(bph.data_type)
  {
    case BPHYS_DATA_SMOKE_LOW:
      read_smoke_data(f);
      break;
  }
}

void BPhysBaker::read_smoke_data(FILE *f)
{
  cout<<"reading smoke sim data..."<<endl;

  char unknown_data[11];
  fread(unknown_data, sizeof(char), 11, f);

/*
  ptcache_file_write(pf, SMOKE_CACHE_VERSION, 4, sizeof(char));
	ptcache_file_write(pf, &fluid_fields, 1, sizeof(int));
	ptcache_file_write(pf, &sds->active_fields, 1, sizeof(int));
	ptcache_file_write(pf, &sds->res, 3, sizeof(int));
	ptcache_file_write(pf, &sds->dx, 1, sizeof(float));
  */
  char version[5];
  int fluid_fields = -1;

  version[4] = '\0';
  fread(&version[0], sizeof(char), 4, f);
  fread(&fluid_fields, 1, sizeof(int), f);
  cout<<"version: "<<version<<endl;
  cout<<"fluid fields: "<<fluid_fields<<endl;
}
