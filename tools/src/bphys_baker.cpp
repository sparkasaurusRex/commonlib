#include <iostream>
#include <assert.h>
#include <math.h>

#include "math_utility.h"
#include "minilzo.h"
#include "bphys_baker.h"

using namespace Math;
using namespace std;
using namespace Tool;
using namespace Graphics;

void BPhysBaker::init()
{
  cout<<"Initializing Blender Point Cache (Physics) Baker..."<<endl;
  if (lzo_init() != LZO_E_OK)
  {
    cerr<<"BPhysBaker::init() - Failed to initialize lzo compression!"<<endl;
  }
}

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

static int ptcache_file_compressed_read(unsigned char *result, unsigned int len, FILE *f)
{
	int r = 0;
	unsigned char compressed = 0;
	size_t in_len;
#ifdef WITH_LZO
	size_t out_len = len;
#endif
	unsigned char *in;
	//unsigned char *props = MEM_callocN(16 * sizeof(char), "tmp");
  unsigned char *props = new unsigned char[16];

	//ptcache_file_read(pf, &compressed, 1, sizeof(unsigned char));
  fread(&compressed, sizeof(unsigned char), 1, f);
	if (compressed) {
		unsigned int size;
		//ptcache_file_read(pf, &size, 1, sizeof(unsigned int));
    fread(&size, sizeof(unsigned int), 1, f);
		in_len = (size_t)size;
		if (in_len==0) {

		}
		else {
			//in = (unsigned char *)MEM_callocN(sizeof(unsigned char)*in_len, "pointcache_compressed_buffer");
      in = new unsigned char[in_len];
			//ptcache_file_read(pf, in, in_len, sizeof(unsigned char));
      fread(in, sizeof(unsigned char), in_len, f);
#ifdef WITH_LZO
			if (compressed == 1)
				r = lzo1x_decompress_safe(in, (lzo_uint)in_len, result, (lzo_uint *)&out_len, NULL);
#endif
#ifdef WITH_LZMA
			if (compressed == 2) {
				size_t sizeOfIt;
				size_t leni = in_len, leno = len;
				//ptcache_file_read(pf, &size, 1, sizeof(unsigned int));
        fread(&size, sizeof(unsigned int), 1, f);
				sizeOfIt = (size_t)size;
				//ptcache_file_read(pf, props, sizeOfIt, sizeof(unsigned char));
        fread(props, sizeof(unsigned char), sizeOfIt, f);
				r = LzmaUncompress(result, &leno, in, &leni, props, sizeOfIt);
			}
#endif
			//MEM_freeN(in);
      delete in;
		}
	}
	else {
		//ptcache_file_read(pf, result, len, sizeof(unsigned char));
    fread(result, sizeof(unsigned char), len, f);
	}

	//MEM_freeN(props);
  delete props;

	return r;
}

#if 0

typedef struct ModifierData {
	struct ModifierData *next, *prev;

	int type, mode;
	int stackindex, pad;
	char name[64];  /* MAX_NAME */

	/* XXX for timing info set by caller... solve later? (ton) */
	struct Scene *scene;

	char *error;
} ModifierData;

typedef struct SmokeModifierData {
	ModifierData modifier;

	struct SmokeDomainSettings *domain;
	struct SmokeFlowSettings *flow; /* inflow, outflow, smoke objects */
	struct SmokeCollSettings *coll; /* collision objects */
	float time;
	int type;  /* domain, inflow, outflow, ... */
} SmokeModifierData;
#endif


void BPhysBaker::read_smoke_data(FILE *f)
{
  cout<<"reading smoke sim data..."<<endl;

  //WTF DOES THIS BLOCK OF DATA DO?!?!?!
  char unknown_data[11];
  fread(unknown_data, sizeof(char), 11, f);

  char version[5];
  int fluid_fields = -1;
  int active_fields = -1;
  unsigned int res[3] = { -1, -1, -1 };
  float dx = -1.0f;

  version[4] = '\0';
  fread(&version[0], sizeof(char), 4, f);

  //fluid info
  fread(&fluid_fields, 1, sizeof(int), f);
  fread(&active_fields, 1, sizeof(int), f);
  fread(&res, 3, sizeof(unsigned int), f);
  fread(&dx, 1, sizeof(float), f);

  cout<<"version: "<<version<<endl;
  cout<<"fluid fields: "<<fluid_fields<<endl;
  cout<<"active fields: "<<active_fields<<endl;
  cout<<"resolution: "<<res[0]<<"x"<<res[1]<<"x"<<res[2]<<endl;
  cout<<"dx: "<<dx<<endl;

  unsigned int alloc_res = res[0] * res[1] * res[2];
  const unsigned int out_len = (unsigned int)(alloc_res) * sizeof(float);

  float *shadow_voxels = new float[alloc_res];
  float *density_voxels = new float[alloc_res];
  float *heat_voxels = NULL;
  float *heat_old_voxels = NULL;
  float *flame_voxels = NULL;
  float *fuel_voxels = NULL;
  float *react_voxels = NULL;
  float *color_voxels_r = NULL;
  float *color_voxels_g = NULL;
  float *color_voxels_b = NULL;
  float *velocity_voxels_x = new float[alloc_res];
  float *velocity_voxels_y = new float[alloc_res];
  float *velocity_voxels_z = new float[alloc_res];

  float sphere_radius = 0.75f;
  int img_res[2] = { 1024, 512 };

  cout<<"reading shadow voxels..."<<endl;
  ptcache_file_compressed_read((unsigned char *)shadow_voxels, out_len, f); //shadow
  Float2 shadow_range(0.0f, 1.0f);
  splat_voxel_data_onto_sphere_surface(res, shadow_range, shadow_range, shadow_range, sphere_radius, img_res[0], img_res[1], std::string("shadow.tga"), shadow_voxels);

  cout<<"reading density voxels..."<<endl;
  ptcache_file_compressed_read((unsigned char *)density_voxels, out_len, f); //density
  Float2 density_range(0.0f, 1.0f);
  splat_voxel_data_onto_sphere_surface(res, density_range, density_range, density_range, sphere_radius, img_res[0], img_res[1], std::string("density.tga"), density_voxels);

  if(fluid_fields & SM_ACTIVE_HEAT)
  {
    cout<<"reading heat voxels..."<<endl;
    heat_voxels = new float[alloc_res];
    heat_old_voxels = new float[alloc_res];
    ptcache_file_compressed_read((unsigned char *)heat_voxels, out_len, f); //heat
    ptcache_file_compressed_read((unsigned char *)heat_old_voxels, out_len, f); //heat_old

    Float2 heat_range(0.0f, 2.0f);
    Float2 heat_old_range(0.0f, 2.0f);
    splat_voxel_data_onto_sphere_surface(res, heat_range, heat_old_range, heat_range, sphere_radius, img_res[0], img_res[1], std::string("heat.tga"), heat_voxels, heat_old_voxels);
  }
  if(fluid_fields & SM_ACTIVE_FIRE)
  {
    cout<<"reading fire voxels..."<<endl;
    flame_voxels = new float[alloc_res];
    fuel_voxels = new float[alloc_res];
    react_voxels = new float[alloc_res];
    ptcache_file_compressed_read((unsigned char *)flame_voxels, out_len, f); //flame
    ptcache_file_compressed_read((unsigned char *)fuel_voxels, out_len, f); //fuel
    ptcache_file_compressed_read((unsigned char *)react_voxels, out_len, f); //react

    Float2 flame_range(0.0f, 0.1f);
    Float2 fuel_range(0.0f, 0.05f);
    Float2 react_range(0.0f, 0.1f);
    splat_voxel_data_onto_sphere_surface(res, flame_range, fuel_range, react_range, sphere_radius, img_res[0], img_res[1], std::string("flame.tga"), flame_voxels, fuel_voxels, react_voxels);
  }
  if(fluid_fields & SM_ACTIVE_COLORS)
  {
    cout<<"reading color voxels..."<<endl;
    color_voxels_r = new float[alloc_res];
    color_voxels_g = new float[alloc_res];
    color_voxels_b = new float[alloc_res];

    Math::Float2 col_range(0.0f, 1.0f);
    ptcache_file_compressed_read((unsigned char *)color_voxels_r, out_len, f); //r
    ptcache_file_compressed_read((unsigned char *)color_voxels_g, out_len, f); //g
    ptcache_file_compressed_read((unsigned char *)color_voxels_b, out_len, f); //b
  }

  //velocity
  cout<<"reading velocity voxels..."<<endl;
  ptcache_file_compressed_read((unsigned char *)velocity_voxels_x, out_len, f); //vx
  ptcache_file_compressed_read((unsigned char *)velocity_voxels_y, out_len, f); //vy
  ptcache_file_compressed_read((unsigned char *)velocity_voxels_z, out_len, f); //vz

  Float2 vel_range(-0.6f, 0.6f);
  splat_voxel_data_onto_sphere_surface(res, vel_range, vel_range, vel_range, sphere_radius, img_res[0], img_res[1], std::string("vel.tga"), velocity_voxels_x, velocity_voxels_y, velocity_voxels_z);

  if(shadow_voxels)     { delete shadow_voxels; }
  if(density_voxels)    { delete density_voxels; }
  if(heat_voxels)       { delete heat_voxels; }
  if(heat_old_voxels)   { delete heat_old_voxels; }
  if(flame_voxels)      { delete flame_voxels; }
  if(fuel_voxels)       { delete fuel_voxels; }
  if(react_voxels)      { delete react_voxels; }
  if(velocity_voxels_x) { delete velocity_voxels_x; }
  if(velocity_voxels_y) { delete velocity_voxels_y; }
  if(velocity_voxels_z) { delete velocity_voxels_z; }
}

/*void BPhysBaker::write_vertical_voxel_slice(float *voxels,
                                            unsigned int *vox_dim,
                                            Float2 vox_range,
                                            int slice_idx,
                                            int tex_width,
                                            int tex_height,
                                            std::string output_tga_fname)
{
  //fill pixel data
  int num_channels = 3;
  long img_size = tex_width * tex_height * num_channels;
  unsigned char *pixels = new unsigned char[img_size];
  for(int j = 0; j < tex_height; j++)
  {
    for(int i = 0; i < tex_width; i++)
    {
      unsigned int v_idx[3];
      //for(int i )
      int v_idx_actual = v_idx[0] + vox_dim[0] * (v_idx[1] + vox_dim[2] * v_idx[2]);
    }
  }
}*/

void BPhysBaker::splat_voxel_data_onto_sphere_surface(unsigned int *vox_dim,
                                                      Float2 vox_range_r,
                                                      Float2 vox_range_g,
                                                      Float2 vox_range_b,
                                                      float radius,
                                                      int tex_width,
                                                      int tex_height,
                                                      std::string tga_file_name,
                                                      float *voxels_r,
                                                      float *voxels_g,
                                                      float *voxels_b)
{
  //fill pixel data
  int num_channels = 3;
  long img_size = tex_width * tex_height * num_channels;
  unsigned char *pixels = new unsigned char[img_size];
  for(int j = 0; j < tex_height; j++)
  {
    for(int i = 0; i < tex_width; i++)
    {
      //interpret i & j as polar coordinates and convert to (x,y,z) space
      float u = (float)i / (float)tex_width;
      float v = (float)j / (float)tex_height;
      float theta = 2.0f * M_PI * u;
      float phi = (M_PI / 2.0f) + M_PI * (1.0f - v);

      Float3 cartesian = polar_to_cartesian(theta, phi, radius);

      //push this xyz into the [0,1] range
      Float3 old_min(-1.0f, -1.0f, -1.0f);
      Float3 old_max(1.0f, 1.0f, 1.0f);
      Float3 new_min(0.0f, 0.0f, 0.0f);
      Float3 new_max(1.0f, 1.0f, 1.0f);
      cartesian = remap_range(cartesian, old_min, old_max, new_min, new_max);

      unsigned int v_idx[3];
      for(int v_idx_i = 0; v_idx_i < 3; v_idx_i++) { v_idx[v_idx_i] = (unsigned int)(cartesian[v_idx_i] * (float)vox_dim[v_idx_i]); }

      float voxel[3] = { 0.0f, 0.0f, 0.0f };
      int v_idx_actual = v_idx[0] + vox_dim[0] * (v_idx[1] + vox_dim[2] * v_idx[2]);
      voxel[0] = remap_range(voxels_r[v_idx_actual], vox_range_r[0], vox_range_r[1], 0.0f, 1.0f);
      if(voxels_g) { voxel[1] = remap_range(voxels_g[v_idx_actual], vox_range_g[0], vox_range_g[1], 0.0f, 1.0f); }
      else { voxel[1] = voxel[0]; }
      if(voxels_b) { voxel[2] = remap_range(voxels_b[v_idx_actual], vox_range_b[0], vox_range_b[1], 0.0f, 1.0f); }
      else { voxel[2] = voxel[0]; }

      int idx = ((tex_width * j) + i) * num_channels;
      float r = voxel[0];
      float g = voxel[1];
      float b = voxel[2];

      pixels[idx] = (unsigned char)(r * 255.0f);
      pixels[idx + 1] = (unsigned char)(g * 255.0f);
      pixels[idx + 2] = (unsigned char)(b * 255.0f);
    }
  }

  write_tga_from_pixels(tga_file_name, tex_width, tex_height, pixels);
  delete pixels;
}

void BPhysBaker::write_tga_from_pixels(std::string fname, int tex_width, int tex_height, unsigned char *pixels)
{
  int num_channels = 3;
  long img_size = tex_width * tex_height * num_channels;

  //assemble the tga header
  int xa = tex_width % 256;
  int xb = (tex_width - xa) / 256;
  int ya = tex_height % 256;
  int yb = (tex_height - ya) / 256;
  unsigned char header[18] = {0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0};

  //write header and data to file
  FILE *f = fopen(fname.c_str(), "wb");
  if(f)
  {
    fwrite(header, sizeof(unsigned char), 18, f);
    fwrite(pixels, sizeof(unsigned char), img_size, f);
    fclose(f);
  }
}
