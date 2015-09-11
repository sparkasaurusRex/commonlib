#include <vector>
#include "push_button.h"

namespace UI
{
  class ToolBox : public Widget
  {
  public:
    ToolBox();
    ~ToolBox() {}

    void init();
    void show(const bool d = true);
    void translate(Math::Float2 p);
    void render();
    void simulate(const float dt) { }
    void process_event(const SDL_Event &event);
    void add_button(PushButton *pb) { buttons.push_back(pb); }
    void set_button_dim(const Float2 d) { pb_dim = d; }
  protected:
    std::vector<PushButton *> buttons;
    bool horizontal;
    float margin;
    Float2 pb_dim;
  };
};
