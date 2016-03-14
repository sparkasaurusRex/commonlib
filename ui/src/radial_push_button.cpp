#include "radial_push_button.h"

using namespace UI;
using namespace std;
using namespace Math;

RadialPushButton::RadialPushButton(Font *f) : RadialWidget(f)
{
  click_capture = false;
  click_callback = NULL;
  textures[0] = textures[1] = NULL;
  click_rgb = Float3(0.8f, 0.8f, 0.8f);
}

void RadialPushButton::set_click_callback(void (*cb)(const SDL_Event &e))
{
  click_callback = cb;
}

void RadialPushButton::init()
{

}

void RadialPushButton::process_event(const SDL_Event &event)
{
  if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
  {
    cout<<"RadialPushButton::process_event(): mouse_down"<<endl;
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(hit_test(mouse_x, mouse_y))
    {
      cout<<"RadialPushButton::process_event(): click capture"<<endl;
      click_capture = true;
    }
  }
  if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
  {
    cout<<"RadialPushButton::process_event(): mouse_up"<<endl;
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    if(click_capture && hit_test(mouse_x, mouse_y))
    {
      cout<<"RadialPushButton::process_event(): button clicked!"<<endl;
      if(click_callback)
      {
        click_callback(event);
      }
      //cout<<"button clicked!"<<endl;
    }
    click_capture = false;
  }
}

void RadialPushButton::render()
{
  Float2 v1(cos(arc[0]), sin(arc[0]));
  Float2 v2(cos(arc[1]), sin(arc[1]));
  Float2 verts[4];
  verts[0] = center + radii[0] * v1;
  verts[1] = center + radii[1] * v1;
  verts[2] = center + radii[0] * v2;
  verts[3] = center + radii[1] * v2;

  glColor3f(1.0f, 1.0f, 0.0f);
  if(click_capture)
  {
    glColor3f(click_rgb[0], click_rgb[1], click_rgb[2]);
  }
  glBegin(GL_LINES);
    glVertex3f(verts[0][0], verts[0][1], 0.0f);
    glVertex3f(verts[1][0], verts[1][1], 0.0f);

    glVertex3f(verts[1][0], verts[1][1], 0.0f);
    glVertex3f(verts[3][0], verts[3][1], 0.0f);

    glVertex3f(verts[0][0], verts[0][1], 0.0f);
    glVertex3f(verts[2][0], verts[2][1], 0.0f);

    glVertex3f(verts[2][0], verts[2][1], 0.0f);
    glVertex3f(verts[3][0], verts[3][1], 0.0f);
  glEnd();
}
