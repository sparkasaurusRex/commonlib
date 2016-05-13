#include "console.h"

#include <iostream>
#include <algorithm>
#include <string.h>
#include <boost/algorithm/string.hpp>

#include "math_utility.h"

#define EXPOSE_PERCENT_RATE 5.0f

using namespace Math;
using namespace std;

DebugConsole::DebugConsole()
{
  font = NULL;
  pct_exposed = 0.0f;
  active = false;

  command_history_idx = 0;

  bg_color = Float3(0.6f, 0.4f, 1.0f);
  float3_var_names.push_back("console.bg_color");
  float3_vars.push_back(&bg_color);

  bg_opacity = 0.55f;
  float_var_names.push_back("console.bg_opacity");
  float_vars.push_back(&bg_opacity);

  text_color = Float3(0.0f, 0.0f, 0.0f);
  float3_var_names.push_back("console.text_color");
  float3_vars.push_back(&text_color);

  last_tab_complete_idx = -1;
}

DebugConsole::~DebugConsole()
{
  delete font;
}

void DebugConsole::activate(bool a)
{
  active = a;
}

bool DebugConsole::is_active() const
{
  if(active)
  {
    return true;
  }
  else if(pct_exposed < 0.01f)
  {
    return false;
  }
  return true;
}

void DebugConsole::init()
{
  //font = new Font(DEFAULT_CONSOLE_FONT_FACE, DEFAULT_CONSOLE_FONT_SIZE);
  //font->init();

  current_command.clear();
  command_history.clear();
  tab_complete_string.clear();
  last_tab_complete_idx = -1;
}

void DebugConsole::simulate(const float dt)
{
  if(active)
  {
    pct_exposed += dt * EXPOSE_PERCENT_RATE;
  }
  else
  {
    pct_exposed -= dt * EXPOSE_PERCENT_RATE;
  }
  pct_exposed = Math::clamp(pct_exposed, 0.0f, 1.0f);
}

void DebugConsole::receive_char(const char c)
{
  switch(c)
  {
    case 17:
    case 18:
      //traverse_command_history(c);
      break;
    default:
      current_command.push_back(c);
      break;
  }
  tab_complete_string = current_command;
  last_tab_complete_idx = -1;
}

void DebugConsole::backspace()
{
  if(current_command.size() > 0)
  {
    current_command.pop_back();
  }
  tab_complete_string = current_command;
  last_tab_complete_idx = -1;
}

void DebugConsole::execute()
{
  //split string by spaces
  std::vector<std::string> words;
  boost::split(words, current_command, boost::is_any_of(" "), boost::token_compress_on);

  //search single float variables
  std::vector<string>::iterator i;
  i = std::find(float_var_names.begin(), float_var_names.end(), words[0]);
  if(i != float_var_names.end())
  {
    int idx = std::distance(float_var_names.begin(), i);
    float *f = float_vars[idx];
    if(words.size() == 1)
    {
      //just output the current value
      cout<<float_var_names[idx].c_str()<<": "<<*f<<endl;
    }
    if(words.size() == 2)
    {
      //cout<<"idx: "<<idx<<endl;
      float val = (float)atof(words[1].c_str());
      *f = val;
    }
  }

  //search float3 variables
  i = std::find(float3_var_names.begin(), float3_var_names.end(), words[0]);
  if(i != float3_var_names.end())
  {
    int idx = std::distance(float3_var_names.begin(), i);
    Float3 *f = float3_vars[idx];
    if(words.size() == 1)
    {
      cout<<float3_var_names[idx].c_str()<<": "<<*f<<endl;
    }
    else if(words.size() == 4)
    {
      float val[3];
      val[0] = (float)atof(words[1].c_str());
      val[1] = (float)atof(words[2].c_str());
      val[2] = (float)atof(words[3].c_str());

      *f = Float3(val[0], val[1], val[2]);
    }
  }

  i = std::find(boolean_var_names.begin(), boolean_var_names.end(), words[0]);
  if(i != boolean_var_names.end())
  {
    int idx = std::distance(boolean_var_names.begin(), i);
    bool *b = boolean_vars[idx];

    if(words.size() == 2)
    {
      bool val = (atoi(words[1].c_str()) != 0);
      *b = val;
    }
    else if(words.size() == 1)
    {
      //*b = !(*b);
      cout<<boolean_var_names[idx].c_str()<<": ";
      if(*b) { cout<<"true"; } else { cout<<"false"; }
      cout<<endl;
    }
  }

  i = std::find(func_var_names.begin(), func_var_names.end(), words[0]);
  if(i != func_var_names.end())
  {
    int idx = std::distance(func_var_names.begin(), i);
    void (*f)() = func_vars[idx];
    f();
  }

  command_history.push_back(current_command);
  command_history_idx = command_history.size();
  current_command.clear();
  tab_complete_string.clear();
  last_tab_complete_idx = -1;
}

void DebugConsole::render_gl()
{
  if(!is_active()) { return; }

  //animation
  float v_offset = Math::cerp(-0.5f, 0.0f, pct_exposed);

  glUseProgramObjectARB(0);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //first render the backdrop
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0f, v_offset, 0.0f);
  glColor4f(bg_color[0], bg_color[1], bg_color[2], bg_opacity);

  glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.4f);              // Top Left
    glVertex2f( 1.0f, -0.4f);              // Top Right
    glVertex2f( 1.0f, -1.0f);              // Bottom Right
    glVertex2f(-1.0f, -1.0f);              // Bottom Left
  glEnd();

  //now render the text
  glColor3f(text_color[0], text_color[1], text_color[2]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, v_offset * 256, 0.0f);

  char command_line[512];
  sprintf(command_line, "your wish > %s", current_command.c_str());

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  float v_pixels = (float)viewport[3] - 0.25f * (float)viewport[3];
  font->print(10, v_pixels, command_line);
}

void DebugConsole::register_variable(bool *b, const char *name)
{
  std::string n(name);
  boolean_var_names.push_back(n);
  boolean_vars.push_back(b);
}

void DebugConsole::register_variable(float *f, const char *name)
{
  std::string n(name);
  float_var_names.push_back(n);
  float_vars.push_back(f);
}

void DebugConsole::register_variable(Float3 *f, const char *name)
{
  std::string n(name);
  float3_var_names.push_back(n);
  float3_vars.push_back(f);
}

void DebugConsole::register_function(void (*f)(), const char *name)
{
  std::string n(name);
  func_var_names.push_back(n);
  func_vars.push_back(f);
}

void DebugConsole::traverse_command_history(const int dir)
{
  if(command_history.size() > 0)
  {
    //current_command = command_history[command_history_idx];
    command_history_idx = (dir > 0) ? command_history_idx - 1 : command_history_idx + 1;
    if(command_history_idx < 0)
    {
      command_history_idx = 0;
    }
    if(command_history_idx >= (int)command_history.size())
    {
      command_history_idx = command_history.size() - 1;
    }
    current_command = command_history[command_history_idx];
  }
}

void DebugConsole::tab_complete(int depth)
{
  if(tab_complete_string.size() <= 0 || last_tab_complete_idx < -1)
  {
    return;
  }

  if((last_tab_complete_idx + 1) < float_var_names.size())
  {
    for(unsigned int i = last_tab_complete_idx + 1; i < float_var_names.size(); i++)
    {
      if(float_var_names[i].find(tab_complete_string) != std::string::npos)
      {
        current_command = float_var_names[i] + " ";
        last_tab_complete_idx = i;
        return;
      }
    }
  }

  if(last_tab_complete_idx < float_var_names.size())
  {
    last_tab_complete_idx = float_var_names.size();
  }

  for(unsigned int i = float_var_names.size() - last_tab_complete_idx; i < float3_var_names.size(); i++)
  {
    if(float3_var_names[i].find(tab_complete_string) != std::string::npos)
    {
      current_command = float3_var_names[i] + " ";
      last_tab_complete_idx = i + float_var_names.size() + 1;
      return;
    }
  }

  unsigned int idx_offset = float3_var_names.size() + float_var_names.size();
  if(last_tab_complete_idx < idx_offset)
  {
    last_tab_complete_idx = idx_offset;
  }
  for(unsigned int i = idx_offset - last_tab_complete_idx; i < boolean_var_names.size(); i++)
  {
    if(boolean_var_names[i].find(tab_complete_string) != std::string::npos)
    {
      cout<<"i:"<<i<<endl;
      current_command = boolean_var_names[i] + " ";
      last_tab_complete_idx = i + idx_offset + 1;
      return;
    }
  }

  idx_offset += boolean_var_names.size();
  if(last_tab_complete_idx < idx_offset)
  {
    last_tab_complete_idx = idx_offset;
  }
  for(unsigned int i = idx_offset - last_tab_complete_idx; i < func_var_names.size(); i++)
  {
    if(func_var_names[i].find(tab_complete_string) != std::string::npos)
    {
      current_command = func_var_names[i] + " ";
      last_tab_complete_idx = i + func_var_names.size() + 1;
      return;
    }
  }

  //didn't find anything.... :(
  last_tab_complete_idx = -1;
  if(depth == 0) { tab_complete(depth + 1); }
}
