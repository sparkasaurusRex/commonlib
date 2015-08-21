#ifndef __FONT_H__
#define __FONT_H__

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class Font
{
  public:
    //Font();
    Font(const char *fname, unsigned int height);
    ~Font();

    void init();
    void print(float x, float y, const char *fmt, ...);
  private:
    void create_display_list(FT_Face face, char ch, GLuint list_base, GLuint *tex_base);

    float h;                //font height
    char face_fname[256];   //font face name
    GLuint *textures;       //texture ids
    GLuint list_base;       //first display list id
};

//void gl_print(const Font &font, float x, float y, const char *fmt, ...);

#endif // __FONT_H__
