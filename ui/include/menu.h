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

      virtual void init();
      virtual void simulate(const double game_time, const double frame_time) { RectangularWidget::simulate(game_time, frame_time); }
      virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
      virtual void render();
    private:
      std::vector<std::pair<std::string, void (*)(const SDL_Event &e)> > items;
      int selection;
      float item_border;
  };
};

#endif //__MENU_H__
