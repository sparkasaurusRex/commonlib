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
#include "static_mesh.h"

using namespace std;
using namespace VR;
using namespace Math;
using namespace PerlinNoise;
using namespace Graphics;

VRContext vr_context;

class VRGame : public SDLGame
{
private:

  Camera cam[2];
  StaticMesh static_mesh;
  float rot_angle;

  void user_init()
  {
    for (int eye = 0; eye < 2; eye++)
    {
      cam[eye].set_window_dimensions(Float2((float)resolution[0], (float)resolution[1]));
    }

    static_mesh_init();
    rot_angle = 0.0f;
  }

  void static_mesh_init()
  {
    //mesh init
    FILE *f;
    f = fopen("../graphics/data/meshes/test_mesh.brick.bin", "rb");
    assert(f);

    static_mesh.read_from_file(f, true);
    static_mesh.init();

    fclose(f);
  }

  void user_run() {}
  void user_process_event(const SDL_Event &event) {}

  void render_static_mesh()
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDisable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_3D);


    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    GLfloat diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat amb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat light_pos[] = { 0.0f, 2.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glRotatef(rot_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_angle * 0.37f, 0.0f, 0.0f, 1.0f);

    static_mesh.render();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

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

      vr_context.get_eye_camera(eye, &cam[eye]);

      cam[eye].render_setup();

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      render_static_mesh();

      /*
      glPointSize(15.0f);
      glBegin(GL_POINTS);
      //glVertex3f(0.0f, 0.0f, 0.0f);
      for (int i = 0; i < 100; i++)
      {
        glVertex3f(point_cloud[i][0], point_cloud[i][1], point_cloud[i][2]);
      }
      glEnd();
      */

      cam[eye].render_cleanup();

      vr_context.render_release(eye);
    }
    vr_context.finalize_render();
  }

  void game_loop(const double game_time, const double frame_time)
  {
    //if(!paused)
    {
      rot_angle += 10.0f * (float)frame_time;
    }
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