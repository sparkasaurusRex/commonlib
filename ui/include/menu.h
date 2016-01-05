#ifndef __MENU_H__
#define __MENU_H__

#include <vector>
#include <string>

#include "rectangular_widget.h"

namespace UI
{
  class Menu : public RectangularWidget
  {
    public:
      Menu(Font *f = NULL);
      ~Menu();

      void add_menu_item(std::string name, void (*cb)(const SDL_Event &e));

      virtual void init() {}
      virtual void simulate(const float dt) {}
      virtual void process_event(const SDL_Event &event);
      virtual void render();
    private:
      std::vector<std::pair<std::string, void (*)(const SDL_Event &e)> > items;
      int selection;
  };
};

#endif //__MENU_H__
