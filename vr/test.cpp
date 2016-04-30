#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif //_WIN32

#include <iostream>

#include "sdl_game.h"
#include "vr.h"
#include "math_utility.h"
#include "perlin.h"
#include "camera.h"

using namespace std;
using namespace VR;
using namespace Math;
using namespace PerlinNoise;
using namespace Graphics;

VRContext vr_context;

class VRGame : public SDLGame
{
private:

  Camera cam;

  std::vector<Float3> 

  void game_loop(const double game_time, const double frame_time) {}
  void user_init() {}
  void user_run() {}
  void user_process_event(const SDL_Event &event) {}
  void render_gl()
  {

    double game_time = get_game_time();
    //render once for each eye
    vr_context.retrieve_eye_poses();
    
    float r = PerlinNoise::scaled_octave_noise_2d(2.0f, 1.0f, 0.001f, 0.0f, 1.0f, (float)game_time, 0.0f);
    float g = PerlinNoise::scaled_octave_noise_2d(2.0f, 1.0f, 0.001f, 0.0f, 1.0f, (float)game_time, 1.2345f);
    float b = PerlinNoise::scaled_octave_noise_2d(2.0f, 1.0f, 0.001f, 0.0f, 1.0f, (float)game_time, 71.453f);

    for (int eye = 0; eye < 2; eye++)
    {
      vr_context.render_capture(eye);

      vr_context.get_eye_camera(eye, &cam);

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glPointSize(15.0f);
      glBegin(GL_POINTS);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glEnd();

      vr_context.render_release(eye);
    }
    vr_context.finalize_render();
  }
public:
  VRGame() : SDLGame(640, 480, "VR Test") {}
  ~VRGame() {}
};

int main(int argc, char **argv)
{
  vr_context.init();

  VRGame game;
  game.enable_vsync(false);
  game.init();

  vr_context.bind(&game);
  
  game.run();

  vr_context.deinit();

  return 0;
}