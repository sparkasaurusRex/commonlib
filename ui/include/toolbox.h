#include <vector>
#include "push_button.h"

namespace UI
{
  class ToolBox : public RectangularWidget
  {
  public:
    ToolBox();
    ~ToolBox() {}

    virtual void init();
    virtual void show(const bool d = true);
    void translate(Math::Float2 p);
    virtual void render();
    virtual void simulate(const float dt) { }
    virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
    void add_button(PushButton *pb) { buttons.push_back(pb); }
    void set_button_dim(const Math::Float2 d) { pb_dim = d; }
    void make_vertical() { horizontal = false; }
    void make_horizontal() { horizontal = true; }
  protected:
    std::vector<PushButton *> buttons;
    bool horizontal;
    float margin;
    Math::Float2 pb_dim;
  };
};
