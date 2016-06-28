#pragma once

//TODO: this should live elsewhere
enum ConsoleColor
{
  CONSOLE_COLOR_DEFAULT,
  CONSOLE_COLOR_RED,
  CONSOLE_COLOR_GREEN
};

#if defined (_WIN32)
#include <windows.h>
#include <direct.h>
#define GETCWD _getcwd
#define CHDIR _chdir
#define FOPEN(f, fname, mode) fopen_s(&f, fname, mode)
#define stricmp _stricmp

#define SET_TEXT_COLOR(c) { \
  uint32_t color_id; \
  switch (c) \
  { \
  case CONSOLE_COLOR_RED: \
    color_id = 12; \
    break; \
  case CONSOLE_COLOR_GREEN: \
    color_id = 10; \
    break; \
  case CONSOLE_COLOR_DEFAULT: \
    color_id = 7; \
    break; \
  } \
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
  SetConsoleTextAttribute(hConsole, color_id); \
}

#else
#include <unistd.h>
#define GETCWD getcwd
#define CHDIR chdir
#define FOPEN(f, fname, mode) f = fopen(fname, mode)
#define stricmp strcasecmp

void set_text_color(ConsoleColor c) {}
#endif
