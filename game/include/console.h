#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <vector>
#include <string>

#include "math_utility.h"
#include "font.h"

//apple only
//#define DEFAULT_CONSOLE_FONT_FACE     "/Library/Fonts/AmericanTypewriter.ttc"
//#define DEFAULT_CONSOLE_FONT_FACE     "/Library/Fonts/OsakaMono.ttf"
#define DEFAULT_CONSOLE_FONT_FACE     "/Library/Fonts/Andale Mono.ttf"
//#define DEFAULT_CONSOLE_FONT_FACE       "/Library/Fonts/Courier New.ttf"

//#define DEFAULT_CONSOLE_FONT_FACE   "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
//#define DEFAULT_CONSOLE_FONT_FACE "/Users/chandra/Dropbox/code/brick/game/data/fonts/sin_city.ttf"

#define DEFAULT_CONSOLE_FONT_SIZE   10

using namespace std;
using namespace Math;

class DebugConsole
{
  public:
    DebugConsole();
    ~DebugConsole();

    void init();
    void activate(bool a);
    bool is_active() const;
    void receive_char(const char c);
    void backspace();
    void execute();
    void simulate(const float dt);
    void render_gl();

    void set_bg_color(Float3 c) { bg_color = c; }
    void set_text_color(Float3 c) { text_color = c; }

    void register_switch(bool *b, const char *name);
    void register_variable(float *f, const char *name);
    void register_variable(Float3 *f, const char *name);

    void traverse_command_history(const int dir);
  private:

    float                   pct_exposed;
    bool                    active;

    Font                    *font;
    Float3                  bg_color;
    float                   bg_opacity;
    Float3                  text_color;

    string                  current_command;

    int                     command_history_idx;
    std::vector<string>     command_history;

    std::vector<string>     boolean_switch_names;
    std::vector<bool *>     boolean_switches;

    std::vector<string>     float_var_names;
    std::vector<float *>    float_vars;

    std::vector<string>     float3_var_names;
    std::vector<Float3 *>   float3_vars;
};

#endif // __CONSOLE_H__
