#include <iostream>
#include <assert.h>
#include "game_controller.h"

using namespace Game;
using namespace std;

void GameControllerContext::init()
{
  int num_joysticks = SDL_NumJoysticks();
  cout << "GameControllerContext::init(): " << num_joysticks << " game controller detected." << endl;
  int controller_index = 0;
  for (int joy = 0; joy < num_joysticks; ++joy)
  {
    if (!SDL_IsGameController(joy))
    {
      continue;
    }
    if (controller_index >= MAX_GAME_CONTROLLERS)
    {
      break;
    }
    controllers[controller_index] = SDL_GameControllerOpen(joy);
    controller_index++;
  }
}

void GameControllerContext::deinit()
{
  for (int ci = 0; ci < MAX_GAME_CONTROLLERS; ++ci)
  {
    if (controllers[ci])
    {
      SDL_GameControllerClose(controllers[ci]);
    }
  }
}

void GameControllerContext::poll_input(const unsigned int ci, GameControllerState *state)
{
  assert(ci < MAX_GAME_CONTROLLERS);

 // for (int ci = 0; ci < MAX_GAME_CONTROLLERS; ci++)
  //{
    if (controllers[ci] && SDL_GameControllerGetAttached(controllers[ci]))
    {
      // NOTE: We have a controller with index ci.
      state->up = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_DPAD_UP);
      state->down = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
      state->left = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
      state->right = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
      state->start = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_START);
      state->back = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_BACK);
      state->leftShoulder = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
      state->rightShoulder = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
      state->aButton = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_A);
      state->bButton = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_B);
      state->xButton = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_X);
      state->yButton = (bool)SDL_GameControllerGetButton(controllers[ci], SDL_CONTROLLER_BUTTON_Y);

      //analog sticks
      state->stick_lx = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_LEFTX) / GAME_CONTROLLER_AXIS_RANGE;
      state->stick_ly = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_LEFTY) / GAME_CONTROLLER_AXIS_RANGE;
      state->stick_rx = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_RIGHTX) / GAME_CONTROLLER_AXIS_RANGE;
      state->stick_ry = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_RIGHTY) / GAME_CONTROLLER_AXIS_RANGE;

      //triggers
      state->trigger_l = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_TRIGGERLEFT) / GAME_CONTROLLER_AXIS_RANGE;
      state->trigger_r = (float)SDL_GameControllerGetAxis(controllers[ci], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / GAME_CONTROLLER_AXIS_RANGE;

      /*
      cout << "Left Stick: (" << stick_lx << ", " << stick_ly << ")" << endl;
      cout << "Right Stick: (" << stick_rx << ", " << stick_ry << ")" << endl;
      cout << "Left Trigger: " << trigger_l << endl;
      cout << "Right Trigger: " << trigger_r << endl;
      */
    }
    else
    {
      // TODO: This controller is note plugged in.
    }
  //}
}