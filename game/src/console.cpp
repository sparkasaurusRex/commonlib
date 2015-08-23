#include "console.h"

#include <iostream>
#include <algorithm>
#include <string.h>
#include <boost/algorithm/string.hpp>

#include "math_utility.h"

#define EXPOSE_PERCENT_RATE 5.0f

using namespace Math;

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
  font = new Font(DEFAULT_CONSOLE_FONT_FACE, DEFAULT_CONSOLE_FONT_SIZE);
  font->init();

  current_command.clear();
  command_history.clear();
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
}

void DebugConsole::backspace()
{
  if(current_command.size() > 0)
  {
    current_command.pop_back();
  }
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
      float val = atof(words[1].c_str());
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
      val[0] = atof(words[1].c_str());
      val[1] = atof(words[2].c_str());
      val[2] = atof(words[3].c_str());

      *f = Float3(val[0], val[1], val[2]);
    }
  }

  i = std::find(boolean_switch_names.begin(), boolean_switch_names.end(), words[0]);
  if(i != boolean_switch_names.end())
  {
    int idx = std::distance(boolean_switch_names.begin(), i);
    bool *b = boolean_switches[idx];

    if(words.size() == 2)
    {
      bool val = (bool)atoi(words[1].c_str());
      *b = val;
    }
    else if(words.size() == 1)
    {
      //*b = !(*b);
      cout<<boolean_switch_names[idx].c_str()<<": ";
      if(*b) { cout<<"true"; } else { cout<<"false"; }
      cout<<endl;
    }
  }

  command_history.push_back(current_command);
  command_history_idx = command_history.size();
  current_command.clear();
}

void DebugConsole::render_gl()
{
  if(!is_active()) { return; }

  //animation
  float v_offset = Math::cerp(-0.5f, 0.0f, pct_exposed);

  glUseProgramObjectARB(0);

  glPushAttrib(GL_DEPTH_BUFFER_BIT);

  //first render the backdrop
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glTranslatef(0.0f, v_offset, 0.0f);

  //glColor4f(0.6f, 0.4f, 1.0f, 0.55f);
  glColor4f(bg_color[0], bg_color[1], bg_color[2], bg_opacity);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.4f);              // Top Left
    glVertex2f( 1.0f, -0.4f);              // Top Right
    glVertex2f( 1.0f, -1.0f);              // Bottom Right
    glVertex2f(-1.0f, -1.0f);              // Bottom Left
  glEnd();
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  //now render the text
  //glColor3f(0.2f, 0.0f, 0.0f);
  glColor3f(text_color[0], text_color[1], text_color[2]);

  glPushMatrix();
  glLoadIdentity();

  glTranslatef(0.0f, v_offset * 256, 0.0f);

  char command_line[512];
  sprintf(command_line, "your wish > %s", current_command.c_str());

  float v_pixels = 0.25f * (float)viewport[3];

  font->print(10, v_pixels, command_line);

  glPopMatrix();
  glPopAttrib();
}

void DebugConsole::register_switch(bool *b, const char *name)
{
  std::string n(name);
  boolean_switch_names.push_back(n);
  boolean_switches.push_back(b);
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
    if(command_history_idx >= command_history.size())
    {
      command_history_idx = command_history.size() - 1;
    }
    current_command = command_history[command_history_idx];
  }
}
