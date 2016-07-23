#include <vector>
#include <assert.h>
#include "push_button.h"

#define TOOLBOX_DEFAULT_MARGIN_PX 5.0f

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
    virtual void simulate(const double game_time, const double frame_time) { RectangularWidget::simulate(game_time, frame_time); }
    virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
    void add_button(PushButton *pb) { assert(pb); buttons.push_back(pb); }
    void set_button_dim(const Math::Float2 d) { pb_dim = d; }
    void make_vertical() { horizontal = false; }
    void make_horizontal() { horizontal = true; }
  protected:
    std::vector<PushButton *> buttons; //TODO - make generic widgets
    bool horizontal;
    float margin;
    Math::Float2 pb_dim;
  };
};
