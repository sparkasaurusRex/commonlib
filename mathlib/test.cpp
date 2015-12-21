#include <iostream>
#include <assert.h>
#include <time.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>

#include "math_utility.h"
#include "matrix.h"
#include "perlin.h"
#include "quaternion.h"
#include "voronoi_sphere.h"
#include "sdl_game.h"
#include "curve.h"

using namespace std;
using namespace Math;
using namespace PerlinNoise;

enum TestMode
{
  TEST_MODE_VORONOI_2D,
  TEST_MODE_VORONOI_3D,
  TEST_MODE_FUNCTIONS,
  NUM_TEST_MODES
};

class TestApp : public SDLGame
{
public:
  TestApp() : SDLGame(512, 512, "Math Test") { Num_starting_points = 0; rot_angle = 0.0f; function_theta = 0.0f; }
  ~TestApp() {}

  void set_num_verts(int nv) { Num_starting_points = nv; }
private:
  void render_gl()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch(mode)
    {
      case TEST_MODE_VORONOI_3D:
        render_voronoi_3d();
        break;
      case TEST_MODE_VORONOI_2D:
        render_voronoi_2d();
        break;
      case TEST_MODE_FUNCTIONS:
        render_functions();
        break;
      default:
        break;
    }
  }

  void render_voronoi_3d()
  {
    Uint32 ticks = SDL_GetTicks();
    float game_time = (float)ticks;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(0.7f, 0.7f, 0.7f);

    glRotatef(rot_angle, 0.0f, 1.0f, 0.0f);
    rot_angle += 0.3f;

    //render stuff here

    glColor3f(0.7f, 0.1f, 0.0f);
    glBegin(GL_LINES);

      TriangulationSphere *tri = point_cloud.get_triangulation();

      std::vector<Edge3D> *edges = tri->get_edges();
      for(int i = 0; i < edges->size(); i++)
      {
        Edge3D e = (*edges)[i];
        Float3 a = point_cloud.get_point(e.vidx[0]);
        Float3 b = point_cloud.get_point(e.vidx[1]);

        float pct = (float)i / (float)edges->size();
        Float3 red(1.0f, 0.0f, 0.0f);
        Float3 blue(0.0f, 0.0f, 1.0f);
        Float3 col = lerp(red, blue, pct);
        glColor3f(col[0], col[1], col[2]);
        glVertex3f(a[0], a[1], a[2]);
        glVertex3f(b[0], b[1], b[2]);
      }

    glEnd();

    glPointSize(4.0f);
    //glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);

      float bh = point_cloud.get_triangulation()->get_beach_line_height();

      for(int i = 0; i < point_cloud.get_num_points(); i++)
      {
        Float3 p = point_cloud.get_point(i);
        if(p[1] < bh) { glColor3f(1.0f, 0.0f, 0.0f); }
        else { glColor3f(1.0f, 1.0f, 0.0f); }
        
        glVertex3f(p[0], p[1], p[2]);
      }

    glEnd();
    glPopMatrix();
  }

  void render_voronoi_2d() {}

  void render_functions()
  {
    CurveEndPoint cep[3];
    cep[0].p = Float2(0.0f, 0.0f);

    float r1 = (1.0f + sin(function_theta * 1.0f));
    cep[0].t = Float2(cep[0].p[0] + r1 * cos(function_theta * 1.5f), cep[0].p[1] + r1 * sin(function_theta * 1.5f));


    cep[1].p = Float2(0.5f, 1.0f);

    float r2 = (1.0f + sin(function_theta * 1.0f));
    cep[1].t = Float2(cep[1].p[0] + r2 * cos(function_theta), cep[1].p[1] + r2 * sin(function_theta));

    cep[2].p = Float2(1.0f, 0.0f);
    cep[2].t = Float2(0.9f, 0.0f);

    CurveSegmentBezier csc;
    CurveSegmentLerp csl;
    csc.set_endpoints(cep[0], cep[1]);
    csl.set_endpoints(cep[1], cep[2]);

    Curve c;
    c.add_segment(&csc);
    c.add_segment(&csl);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(3.0f);

    glScalef(1.2f, 1.2f, 0.0f);
    glTranslatef(-0.5f, -0.5f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
      glVertex3f(cep[0].p[0], cep[0].p[1], 0.0f);
      glVertex3f(cep[1].p[0], cep[1].p[1], 0.0f);
      glVertex3f(cep[2].p[0], cep[2].p[1], 0.0f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
      glVertex3f(cep[0].t[0], cep[0].t[1], 0.0f);
      glVertex3f(cep[1].t[0], cep[1].t[1], 0.0f);
      glVertex3f(cep[2].t[0], cep[2].t[1], 0.0f);
    glEnd();

    glBegin(GL_LINES);
      glVertex3f(cep[0].p[0], cep[0].p[1], 0.0f);
      glVertex3f(cep[0].t[0], cep[0].t[1], 0.0f);

      glVertex3f(cep[1].p[0], cep[1].p[1], 0.0f);
      glVertex3f(cep[1].t[0], cep[1].t[1], 0.0f);

      glVertex3f(cep[2].p[0], cep[2].p[1], 0.0f);
      glVertex3f(cep[2].t[0], cep[2].t[1], 0.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    int num_slices = 100;
    glBegin(GL_LINE_STRIP);
      for(int i = 0; i < num_slices + 1; i++)
      {
        float x = (float)i / (float)num_slices;
        float y = c.evaluate(x);

        glVertex3f(x, y, 0.0f);
      }
    glEnd();

    //draw bounding box
    glBegin(GL_LINE_STRIP);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 0.0f);
      glVertex3f(1.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
  }


  void game_loop(const float game_time, const float frame_time)
  {
    function_theta += frame_time;
    if(function_theta > M_PI * 2.0f) { function_theta -= M_PI * 2.0f; }

    point_cloud.triangulation_step(0.01f);
  }

  void user_init()
  {
    mode = TEST_MODE_VORONOI_3D;
    for(int i = 0; i < Num_starting_points; i++)
    {
      Float3 p(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(-1.0f, 1.0f));
      p.normalize();
      point_cloud.add_point(p);
    }
    point_cloud.triangulate();
  }
  void user_run() {}
  void user_process_event(const SDL_Event &event)
  {
    switch(event.type)
    {
      case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
          case ']':
            mode = (TestMode)((int)mode + 1);
            if(mode >= NUM_TEST_MODES) { mode = TEST_MODE_VORONOI_2D; }
            break;
          case '[':
            mode = (TestMode)((int)mode - 1);
            if(mode < 0) { mode = TEST_MODE_FUNCTIONS; }
            break;
          default:
            break;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if(mode == TEST_MODE_VORONOI_3D)
        {
          Float3 new_vertex;
          cout<<"click: ("<<event.button.x<<", "<<event.button.y<<")"<<endl;
          new_vertex[0] = 2.0f * (((float)event.button.x / (float)resolution[0]) - 0.5f);
          new_vertex[1] = -2.0f * (((float)event.button.y / (float)resolution[1]) - 0.5f);
          new_vertex[2] = random(-1.0f, 1.0f);
          new_vertex.normalize();
          point_cloud.add_point(new_vertex);
          point_cloud.triangulate();
        }
        break;
    }
  }

  //variables
  VoronoiSphere point_cloud;
  int Num_starting_points;
  float rot_angle;
  TestMode mode;

  float function_theta;
};


int main(int argc, char **argv)
{
  TestApp app;

  srand(time(NULL));

  if(argc > 1)
  {
    app.set_num_verts(atoi(argv[1]));
  }

  app.init();
  Float3 vec_test(1.5f, 0.4f, -21.4f);

  cout<<vec_test.magnitude()<<endl;

  cout<<raw_noise_2d(4.5f, 1.23f)<<endl;

  //test perlin
  float p = PerlinNoise::octave_noise_2d(1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
  cout<<p<<endl;

  Matrix2x2 m;
  m.rotation(0);

  cout<<"rotation_matrix:"<<endl;
  cout<<m<<endl;

  Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
  Quaternion q2(2.0f, 3.0f, 4.0f, 5.0f);
  cout<<"quaternions: "<<endl;
  cout<<"Q1: "<<q1<<endl;
  cout<<"Q2: "<<q2<<endl;
  cout<<"Q1 * Q2: "<<q1*q2<<endl;
  cout<<"conjugate of Q1: "<<~q1<<endl;
  cout<<"norm of Q1: "<<sqrt(q1.norm_squared())<<endl;
  q2.normalize();
  cout<<"normalized Q2: "<<q2<<endl<<endl;

  cout<<"quaternion from axis / angle rotation..."<<endl;
  Quaternion rot_quat;
  Float3 rot_axis(1.0f, 0.0f, 0.0f);
  rot_quat.rotation_from_axis_angle(rot_axis, M_PI * 0.137f);
  rot_quat.normalize();
  cout<<"rot_quat:"<<endl<<rot_quat<<endl;

  Matrix3x3 m3;
  cout<<"rotation from quaternion rot_quat: "<<endl;
  m3.rotation_from_quaternion(rot_quat);
  cout<<m3<<endl;
  cout<<"inverting matrix..."<<endl;
  m3.invert();
  cout<<m3<<endl;

  app.run();

  return 0;
}
