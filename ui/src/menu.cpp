#include "menu.h"

using namespace UI;
using namespace std;

Menu::Menu(Font *f) : RectangularWidget(f) {}

Menu::~Menu() {}

void Menu::add_menu_item(string name, void (*cb)(const SDL_Event &e))
{
  pair<string, void (*)(const SDL_Event &e)>p(name, cb);
  items.push_back(p);
}

void Menu::process_event(const SDL_Event &event)
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
          show(false);
          break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      break;
    case SDL_MOUSEBUTTONUP:
      break;
  }

  if(selection < 0) { selection = items.size() - 1; }
  if(selection >= items.size()) { selection = 0; }
}

void Menu::render()
{
  if(!visible) { return; }

  float font_height = 15.0f;
  float height = pos[1];
  for(int i = 0; i < items.size(); i++)
  {
    if(selection == i)
    {
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    else
    {
      glColor3f(1.0f, 1.0f, 1.0f);
    }
    font->print(pos[0], height, items[i].first.c_str());
    height += font_height;
  }
}
