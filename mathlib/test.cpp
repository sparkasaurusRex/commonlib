#include <iostream>
#include <assert.h>

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

using namespace std;
using namespace Math;
using namespace PerlinNoise;

const int WIN_WIDTH = 512;
const int WIN_HEIGHT = 512;

SDL_Window* sdl_window = NULL;
SDL_GLContext sdl_gl_context = NULL;

void quit_app()
{
    SDL_GL_DeleteContext(sdl_gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    exit(0);
}

void game_loop()
{
	Uint32 ticks = SDL_GetTicks();
	float game_time = (float)ticks;

	//render stuff here
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5.0f);

	glBegin(GL_POINTS);

		for(int i = 0; i < 100; i++)
		{
			glVertex3f(random(-1.0f, 1.0f), random(-1.0f, 1.0f), 0.0f);
		}

	glEnd();

	glFlush();
	SDL_GL_SwapWindow(sdl_window);
}

void init_sdl()
	{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  }
  else
  {
	  //Create window
	  sdl_window = SDL_CreateWindow("Graphics Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	  assert(sdl_window);

	  sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	  assert(sdl_gl_context);

	  SDL_GL_SetSwapInterval(1);
  }
}

void init_gl()
{

}

void process_events()
{
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  if(keystate[SDLK_ESCAPE]) quit_app();

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      quit_app();
    }
  }
}

int main(int argc, char **argv)
{
	init_sdl();
	init_gl();

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
	rot_quat.rotation_from_axis_angle(rot_axis, M_PI * 0.25f);
	rot_quat.normalize();
	cout<<"rot_quat:"<<endl<<rot_quat<<endl;

	Matrix3x3 m3;
	cout<<"rotation from quaternion rot_quat: "<<endl;
	m3.rotation_from_quaternion(rot_quat);
	cout<<m3<<endl;

	while(true)
	{
		process_events();
		game_loop();
	}

	quit_app();

	return 0;
}
