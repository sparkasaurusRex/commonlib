#include <iostream>
#include <assert.h>
#include <lzo/lzo1x.h>
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

/*
static int ptcache_file_compressed_read(PTCacheFile *pf, unsigned char *result, unsigned int len)
{
	int r = 0;
	unsigned char compressed = 0;
	size_t in_len;
#ifdef WITH_LZO
	size_t out_len = len;
#endif
	unsigned char *in;
	unsigned char *props = MEM_callocN(16 * sizeof(char), "tmp");

	ptcache_file_read(pf, &compressed, 1, sizeof(unsigned char));
	if (compressed) {
		unsigned int size;
		ptcache_file_read(pf, &size, 1, sizeof(unsigned int));
		in_len = (size_t)size;
		if (in_len==0) {

		}
		else {
			in = (unsigned char *)MEM_callocN(sizeof(unsigned char)*in_len, "pointcache_compressed_buffer");
			ptcache_file_read(pf, in, in_len, sizeof(unsigned char));
#ifdef WITH_LZO
			if (compressed == 1)
				r = lzo1x_decompress_safe(in, (lzo_uint)in_len, result, (lzo_uint *)&out_len, NULL);
#endif
#ifdef WITH_LZMA
			if (compressed == 2) {
				size_t sizeOfIt;
				size_t leni = in_len, leno = len;
				ptcache_file_read(pf, &size, 1, sizeof(unsigned int));
				sizeOfIt = (size_t)size;
				ptcache_file_read(pf, props, sizeOfIt, sizeof(unsigned char));
				r = LzmaUncompress(result, &leno, in, &leni, props, sizeOfIt);
			}
#endif
			MEM_freeN(in);
		}
	}
	else {
		ptcache_file_read(pf, result, len, sizeof(unsigned char));
	}

	MEM_freeN(props);

	return r;
}
*/

void BPhysBaker::read_smoke_data(FILE *f)
{
  cout<<"reading smoke sim data..."<<endl;

  //WTF DOES THIS BLOCK OF DATA DO?!?!?!
  char unknown_data[11];
  fread(unknown_data, sizeof(char), 11, f);

  char version[5];
  int fluid_fields = -1;
  int active_fields = -1;
  int res = -1;
  float dx = -1.0f;

  version[4] = '\0';
  fread(&version[0], sizeof(char), 4, f);
  fread(&fluid_fields, 1, sizeof(int), f);
  fread(&active_fields, 1, sizeof(int), f);
  fread(&res, 1, sizeof(int), f);
  fread(&dx, 1, sizeof(float), f);

  cout<<"version: "<<version<<endl;
  cout<<"fluid fields: "<<fluid_fields<<endl;
  cout<<"active fields: "<<active_fields<<endl;
  cout<<"resolution: "<<res<<endl;
  cout<<"dx: "<<dx<<endl;
}
