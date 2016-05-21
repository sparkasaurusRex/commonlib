#include "menu.h"
#include "math_utility.h"

using namespace UI;
using namespace std;
using namespace Math;

Menu::Menu(Font *f) : RectangularWidget(f)
{
  selection = 0;
  item_border = 10.0f;
}

Menu::~Menu() {}

void Menu::add_menu_item(string name, void (*cb)(const SDL_Event &e))
{
  pair<string, void (*)(const SDL_Event &e)>p(name, cb);
  items.push_back(p);
}

void Menu::init()
{
  //update dimensions
  dim[1] = (float)items.size() * font->get_height() + item_border;

  float widest = 0.0f;
  for(unsigned int i = 0; i < items.size(); i++)
  {
    char buffer[256];
    strcpy(buffer, items[i].first.c_str());
    widest = max(widest, font->get_string_width(buffer));
  }
  dim[0] = widest;
}

void Menu::process_event(const SDL_Event &event, const Float2 offset)
{
  if(!visible) { return; }

  switch(event.type)
  {
    case SDL_KEYUP:
      switch(event.key.keysym.sym)
      {
        case SDLK_UP:
          selection--;
          break;
        case SDLK_DOWN:
          selection++;
          break;
        case SDLK_RETURN:
          items[selection].second(event);
          break;
        case SDLK_ESCAPE:
          visible = false;
          break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      break;
    case SDL_MOUSEBUTTONUP:
      break;
  }

  if(items.size() <= 0) { return;  }
  if(selection < 0) { selection = (unsigned int)items.size() - 1; }
  if(selection >= (int)items.size()) { selection = 0; }
}

void Menu::render()
{
  if(!visible) { return; }

  float font_height = font->get_height() + item_border;
  float height = pos[1];
  for(unsigned int i = 0; i < items.size(); i++)
  {
    if(selection == i)
    {
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    else
    {
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    char buffer[256];
    strcpy(buffer, items[i].first.c_str());
    float width = font->get_string_width(buffer);
    float x = pos[0] + dim[0] / 2.0f - width / 2.0f;

    font->print(x, height, items[i].first.c_str());
    height += font_height;
  }
}
