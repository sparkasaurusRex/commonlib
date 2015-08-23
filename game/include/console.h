#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <vector>
#include <string>


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
    void register_switch(bool *b, const char *name);
    void traverse_command_history(const int dir);
  private:

    float pct_exposed;
    bool active;

    Font *font;
    string current_command;

    int command_history_idx;
    std::vector<string> command_history;

    std::vector<string> boolean_switch_names;
    std::vector<bool *> boolean_switches;
};

#endif // __CONSOLE_H__
