#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "widget_wrangler.h"

using namespace UI;

void WidgetWrangler::render() const
{
  //glPushAttrib(GL_TRANSFORM_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(viewport[0],viewport[2],viewport[3],viewport[1]);
      //gluOrtho2D(viewport[0], viewport[3], viewport[1], viewport[2]);
      //glPopAttrib();

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      for(int i = 0; i < widgets.size(); i++)
      {
        widgets[i]->render();
      }

      //glPushAttrib(GL_TRANSFORM_BIT);
      //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
  //glPopAttrib();
}
