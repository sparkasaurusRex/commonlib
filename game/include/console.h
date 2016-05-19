#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <vector>
#include <string>

#include "math_utility.h"
#include "font.h"
#include "widget_wrangler.h"
#include "meter.h"

#define DEFAULT_CONSOLE_FONT_SIZE   10

enum ConsoleState
{
  CONSOLE_INACTIVE,
  CONSOLE_ACTIVE_DEFAULT,
  CONSOLE_ACTIVE_CONTROL_BOARD
};

class DebugConsole
{
private:
  float                       pct_exposed;
  ConsoleState                state;

  Font                        *font;
  Math::Float3                bg_color;
  float                       bg_opacity;
  Math::Float3                text_color;

  std::string                 current_command;
  std::string                 console_buffer;

  int                         command_history_idx;
  std::vector<std::string>    command_history;

  std::vector<std::string>    boolean_var_names;
  std::vector<bool *>         boolean_vars;

  std::vector<std::string>    float_var_names;
  std::vector<float *>        float_vars;
  std::vector<Math::Float2 >  float_var_ranges;
  std::vector<UI::Meter *>    float_var_sliders;

  std::vector<std::string>    float3_var_names;
  std::vector<Math::Float3 *> float3_vars;

  std::vector<std::string>    func_var_names;
  std::vector<void(*)()>      func_vars;

  //tab completion
  std::string                 tab_complete_string;
  unsigned int                last_tab_complete_idx;

  UI::WidgetWrangler          console_ww;

public:
  DebugConsole();
  ~DebugConsole();

  void init();
  void activate(ConsoleState s);
  bool is_active() const;
  bool is_control_board_active() const;
  void receive_char(const char c);
  void backspace();
  void execute();
  void simulate(const float dt);

  void print_line(std::string s);
  
  void render_gl();
  void render_default();
  void render_control_board();

  void set_bg_color(Math::Float3 c) { bg_color = c; }
  void set_text_color(Math::Float3 c) { text_color = c; }

  void register_variable(bool *b, const char *name);
  void register_variable(float *f, const char *name, const Math::Float2 range = Math::Float2(0.0f, 1.0f));
  void register_variable(Math::Float3 *f, const char *name);
  void register_function(void (*f)(), const char *name);

  void traverse_command_history(const int dir);
  void tab_complete(int depth = 0);

	void set_font(Font *f) { font = f; }
};

#endif // __CONSOLE_H__
