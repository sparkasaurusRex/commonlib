#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#if defined(_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif //_WIN32

#define MAX_GAME_CONTROLLERS 4
#define GAME_CONTROLLER_AXIS_RANGE 32767

namespace Game
{
  struct GameControllerState
  {
    bool up;
    bool down;
    bool left;
    bool right;
    bool start;
    bool back;
    bool leftShoulder;
    bool rightShoulder;
    bool aButton;
    bool bButton;
    bool xButton;
    bool yButton;

    //analog sticks
    float stick_lx;
    float stick_ly;
    float stick_rx;
    float stick_ry;

    //triggers
    float trigger_l;
    float trigger_r;
  };

  class GameControllerContext
  {
  private:
    SDL_GameController *controllers[MAX_GAME_CONTROLLERS];
  public:
    GameControllerContext() {}
    ~GameControllerContext() {}

    void init();
    void deinit();

    void poll_input(const unsigned int ci, GameControllerState *state);
  };
};

#endif //__GAME_CONTROLLER_H__