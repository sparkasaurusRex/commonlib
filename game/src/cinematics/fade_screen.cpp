#include <iostream>
#include "cinematics/fade_screen.h"
#include "math_utility.h"

using namespace std;
using namespace Math;

FadeScreen::FadeScreen()
{
  fade_in_timer.set(1.0f);
  linger_timer.set(1.0f);
  fade_out_timer.set(1.0f);

  font = NULL;

  fade_opacity = 0.0f;
  backdrop_texture = NULL;
}

void FadeScreen::set_font(Font *f)
{
  font = f;
}

void FadeScreen::simulate(const float dt)
{
  if(!fade_in_timer.elapsed())
  {
    //cout<<"fade in"<<endl;
    fade_opacity = fade_in_timer.pct_elapsed();
    if(fade_opacity >= 1.0f)
    {
      fade_opacity = 1.0f;
      linger_timer.set(5.0);
    }
  }
  else if(!linger_timer.elapsed())
  {
    //cout<<"linger"<<endl;
    fade_opacity = 1.0f;
    if(linger_timer.pct_elapsed() >= 1.0f)
    {
      fade_out_timer.set(1.0);
    }
  }
  else if(!fade_out_timer.elapsed())
  {
    //cout<<"fade out"<<endl;
    fade_opacity = clamp(1.0f - fade_out_timer.pct_elapsed(), 0.0f, 1.0f);
  }
  //cout<<"title screen opacity: "<<fade_opacity<<endl;
}

void FadeScreen::render_gl() const
{
  //glPushAttrib(GL_DEPTH_BUFFER_BIT);

  //first render the backdrop
  glMatrixMode(GL_PROJECTION);
  //glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  //glPushMatrix();
  glLoadIdentity();

  glColor4f(1.0f, 1.0f, 1.0f, fade_opacity);

  //render backdrop Texture
  if(backdrop_texture)
  {
    backdrop_texture->render_gl();

    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
    //backdrop_texture->render_cleanup();
  }
  else
  {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float h = font->get_height();
    char foo[256];
    strcpy(foo, text.c_str());
    float w = font->get_string_width(foo);
    //cout<<"font width: "<<w<<endl;
    font->print((viewport[2] - w) / 2.0f, (viewport[3] - h) / 2.0f, text.c_str());

  }
 // glPopMatrix();
  //glPopMatrix();
  //glPopAttrib();
}

void FadeScreen::play()
{
  fade_in_timer.set(1.0);
}

bool FadeScreen::is_active() const
{
  if(!fade_in_timer.elapsed()) return true;
  if(!linger_timer.elapsed()) return true;
  if(!fade_out_timer.elapsed()) return true;
  return false;
}
