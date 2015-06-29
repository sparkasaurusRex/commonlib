#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "static_mesh.h"

class Texture
{
public:
	Texture(const int w = 32, const int h = 32);
	Texture(const char *n);

	~Texture();

	void get_dim(int &w, int &h) const { w = dim[0]; h = dim[1]; }
	void set_dim(const int w, const int h) { dim[0] = w; dim[1] = h; }

	void init();

	bool load();
	bool load_from_file_data(TextureFileData &tfd);
	bool render_gl() const;

	bool update_pixels_from_mem(void *pixels, int bpp_mode);

private:
	int			dim[2];			//dimensions of the texture
	char		fname[256];		//filename
	GLuint		gl_texture;		//OpenGL texture name
	GLuint		gl_mode;		//image format
};

#endif // _TEXTURE_H_
