#include "widget.h"

using namespace UI;

void Widget::render_prep()
{
  //TODO: UI renderer / context that preps all this before rendering all UI components
  glPushAttrib(GL_DEPTH_BUFFER_BIT);

  //first render the backdrop
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
}

void Widget::render_cleanup()
{
  glPopMatrix();
  glPopMatrix();
  glPopAttrib();
}
