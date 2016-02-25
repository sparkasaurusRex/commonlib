#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "widget_wrangler.h"

using namespace UI;
using namespace std;

WidgetWrangler::WidgetWrangler()
{
  focus_idx = 0;
}

void WidgetWrangler::render() const
{
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(viewport[0],viewport[2],viewport[3],viewport[1]);

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glDepthMask(GL_FALSE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  for(int i = 0; i < widgets.size(); i++)
  {
    widgets[i]->render();
  }
}

void WidgetWrangler::process_event(const SDL_Event &e)
{
  //cout<<"WidgetWrangler::process_event()"<<endl;
  //first, figure out which widget has the focus
  //focus_idx = 0;
  for(int i = 0; i < widgets.size(); i++)
  {
    Widget *w = widgets[i];
    if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
    {
      int mouse_x, mouse_y;
      Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
      if(w->hit_test(mouse_x, mouse_y))
      {
        //w->process_event(e);
        focus_idx = i;
        cout<<"focus_id: "<<focus_idx<<endl;
        //cout<<"pos: "<<w->get_pos()<<endl;
      }
    }
  }

  //next, dispatch event messages
  for(int i = 0; i < widgets.size(); i++)
  {
    Widget *w = widgets[i];
    //w->process_event(e);
    if(i == focus_idx)
    {
      w->set_focus(true);
    }
    else
    {
      w->set_focus(false);
    }
  }

  Widget *focused = widgets[focus_idx];
  focused->process_event(e);
}

void WidgetWrangler::simulate(const float dt)
{
  for(int i = 0; i < widgets.size(); i++)
  {
    widgets[i]->simulate(dt);
  }
}
