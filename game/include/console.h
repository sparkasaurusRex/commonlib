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

    void set_bg_color(Math::Float3 c) { bg_color = c; }
    void set_text_color(Math::Float3 c) { text_color = c; }

    void register_variable(bool *b, const char *name);
    void register_variable(float *f, const char *name);
    void register_variable(Math::Float3 *f, const char *name);
    void register_function(void (*f)(), const char *name);

    void traverse_command_history(const int dir);
    void tab_complete(int depth = 0);

  private:
    float                       pct_exposed;
    bool                        active;

    Font                        *font;
    Math::Float3                bg_color;
    float                       bg_opacity;
    Math::Float3                text_color;

    std::string                 current_command;

    unsigned int                command_history_idx;
    std::vector<std::string>    command_history;

    std::vector<std::string>    boolean_var_names;
    std::vector<bool *>         boolean_vars;

    std::vector<std::string>    float_var_names;
    std::vector<float *>        float_vars;

    std::vector<std::string>    float3_var_names;
    std::vector<Math::Float3 *> float3_vars;

    std::vector<std::string>    func_var_names;
    std::vector<void (*)()>     func_vars;

    //tab completion
    std::string                 tab_complete_string;
    unsigned int                last_tab_complete_idx;
};

#endif // __CONSOLE_H__
