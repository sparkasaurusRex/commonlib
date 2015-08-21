#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "font.h"

using namespace std;

/*
Largely taken from NeHe:
http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
*/

inline int next_p2(int a)
{
  int rval = 1;
  while(rval < a)
  {
    rval<<=1;
  }
  return rval;
}

// A Fairly Straightforward Function That Pushes
// A Projection Matrix That Will Make Object World
// Coordinates Identical To Window Coordinates.
inline void pushScreenCoordinateMatrix()
{
  glPushAttrib(GL_TRANSFORM_BIT);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
  glPopAttrib();
}

// Pops The Projection Matrix Without Changing The Current
// MatrixMode.
inline void pop_projection_matrix()
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

void Font::print(float x, float y, const char *fmt, ...)
{
  // We Want A Coordinate System Where Distance Is Measured In Window Pixels.
  pushScreenCoordinateMatrix();

  GLuint font = list_base;
  // We Make The Height A Little Bigger.  There Will Be Some Space Between Lines.
  float height = h / 0.63f;
  char text[256];                                  // Holds Our String
  va_list ap;                                     // Pointer To List Of Arguments

  if(fmt == NULL)                                    // If There's No Text
      *text = 0;                                    // Do Nothing
  else {
      va_start(ap, fmt);                              // Parses The String For Variables
      vsprintf(text, fmt, ap);                            // And Converts Symbols To Actual Numbers
      va_end(ap);                                 // Results Are Stored In Text
  }

  // Here Is Some Code To Split The Text That We Have Been
  // Given Into A Set Of Lines.
  // This Could Be Made Much Neater By Using
  // A Regular Expression Library Such As The One Available From
  // boost.org (I've Only Done It Out By Hand To Avoid Complicating
  // This Tutorial With Unnecessary Library Dependencies).
  const char *start_line=text;
  vector<string> lines;
  const char *c;// = text;
  for(c = text; *c; c++) {
    if(*c == '\n') {
      string line;
      for(const char *n = start_line; n < c; n++) line.append(1,*n);
      lines.push_back(line);
      start_line = c + 1;
    }
  }
  if(start_line) {
    string line;
    for(const char *n = start_line; n < c; n++) line.append(1,*n);
    lines.push_back(line);
  }

  glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glListBase(font);

  float modelview_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

  // This Is Where The Text Display Actually Happens.
  // For Each Line Of Text We Reset The Modelview Matrix
  // So That The Line's Text Will Start In The Correct Position.
  // Notice That We Need To Reset The Matrix, Rather Than Just Translating
  // Down By h. This Is Because When Each Character Is
  // Drawn It Modifies The Current Matrix So That The Next Character
  // Will Be Drawn Immediately After It.
  for(int i = 0; i < lines.size(); i++) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y - height * i, 0);
    glMultMatrixf(modelview_matrix);

// The Commented Out Raster Position Stuff Can Be Useful If You Need To
// Know The Length Of The Text That You Are Creating.
// If You Decide To Use It Make Sure To Also Uncomment The glBitmap Command
// In make_dlist().
    // glRasterPos2f(0,0);
    glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
    // float rpos[4];
    // glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
    // float len=x-rpos[0]; (Assuming No Rotations Have Happend)

    glPopMatrix();
  }

  glPopAttrib();

  pop_projection_matrix();
}

Font::Font(const char *fname, unsigned int height)
{
  h = height;
  strcpy(face_fname, fname);
}

Font::~Font()
{
  glDeleteLists(list_base, 128);
  glDeleteTextures(128, textures);
  delete [] textures;
}

void Font::init()
{
  cout<<"initializing font "<<face_fname<<endl;

  // Allocate Some Memory To Store The Texture Ids.
  textures = new GLuint[128];

  FT_Library library;
  FT_Error error;
  error = FT_Init_FreeType(&library);
  assert(error == 0);

  // The Object In Which FreeType Holds Information On A Given
  // Font Is Called A "face".
  FT_Face face;

  // This Is Where We Load In The Font Information From The File.
  // Of All The Places Where The Code Might Die, This Is The Most Likely,
  // As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
  error = FT_New_Face(library, face_fname, 0, &face);
  assert(error == 0);

  // For Some Twisted Reason, FreeType Measures Font Size
  // In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
  // h Pixels High, We Need To Request A Size Of h*64.
  // (h << 6 Is Just A Prettier Way Of Writing h*64)
  FT_Set_Char_Size(face, (int)h << 6, (int)h << 6, 96, 96);

  // Here We Ask OpenGL To Allocate Resources For
  // All The Textures And Display Lists Which We
  // Are About To Create.
  list_base = glGenLists(128);
  glGenTextures(128, textures);

  // This Is Where We Actually Create Each Of The Fonts Display Lists.
  for(unsigned char i = 0; i < 128; i++)
  {
    create_display_list(face, i, list_base, textures);
  }

  //clean up the free type objects
  FT_Done_Face(face);
  FT_Done_FreeType(library);
}

//create an openGL display list using FreeType to create a bitmap
void Font::create_display_list(FT_Face face, char ch, GLuint list_base, GLuint *tex_base)
{
  //First, load the glyph for the character
  FT_Error error;
  error = FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
  assert(error == 0);

  //put that in a glyph object
  FT_Glyph glyph;
  error = FT_Get_Glyph(face->glyph, &glyph);
  assert(error == 0);

  //convert the glyph to a bitmap
  FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
  FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

  FT_Bitmap &bitmap = bitmap_glyph->bitmap;

  //compute dimensions that are powers of two (for OpenGL)
  int width = next_p2(bitmap.width);
  int height = next_p2(bitmap.rows);

  float vertical_offset = (float)bitmap_glyph->top - (float)bitmap.rows;

  //allocate new data and fill
  GLubyte* expanded_data = new GLubyte[2 * width * height];
  for(int j = 0; j < height;j++)
  {
    for(int i = 0; i < width; i++)
    {
      expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] =
            (i >= bitmap.width || j >= bitmap.rows) ?
            0 :
            bitmap.buffer[i + bitmap.width * j];
    }
  }

  //create the opengl texture
  glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

  //done w/ that shiz
  delete [] expanded_data;

  //a'ight, now we gonna create a display list for our glyph
  glNewList(list_base + ch, GL_COMPILE);
  glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
  glPushMatrix();

  //move over a little, and down by whatever we need to
  glTranslatef(bitmap_glyph->left, 0, 0);
  glTranslatef(0, vertical_offset, 0);

  //account for padding space in the texture
  float x = (float)bitmap.width / (float)width,
  y = (float)bitmap.rows / (float)height;

  //draw the textured quad
  glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex2f(0, bitmap.rows);
    glTexCoord2d(0, y); glVertex2f(0, 0);
    glTexCoord2d(x, y); glVertex2f(bitmap.width, 0);
    glTexCoord2d(x, 0); glVertex2f(bitmap.width, bitmap.rows);
  glEnd();

  glPopMatrix();

  glTranslatef(face->glyph->advance.x >> 6, 0, 0);

  glEndList();
}
