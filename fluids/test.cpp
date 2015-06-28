#include <iostream>
#include <assert.h>
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>

#include "fluid2d.h"

using namespace std;

const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

SDL_Window* sdl_window = NULL;
SDL_GLContext sdl_gl_context = NULL;

void init_gl()
{
    GLenum res = glewInit();
    assert(res == GLEW_OK);

    cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<endl;

    char *extensions = (char *)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);
    strcpy(extensions, (char *)glGetString(GL_EXTENSIONS));
    int len = strlen(extensions);
    for(int i = 0; i < len; i++) {
        if(extensions[i] == ' ') {
            extensions[i] = '\n';
        }
    }
    //cout<<extensions<<endl;
    free(extensions);


    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    

    glMatrixMode(GL_PROJECTION);
/*#if defined(__APPLE__)
    GLKMatrix4MakePerspective
#else*/
    //gluPerspective(32, (float)WIN_WIDTH / WIN_HEIGHT, 0.05f, 1000.0f);
//#endif
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
}

void init_sdl()
{
    //Initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    else
    {
        //Create window
        sdl_window = SDL_CreateWindow("Mundus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        assert(sdl_window);

        sdl_gl_context = SDL_GL_CreateContext(sdl_window);
        assert(sdl_gl_context);

        SDL_GL_SetSwapInterval(1);
    }
}

void quit_app()
{
    SDL_GL_DeleteContext(sdl_gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    exit(0);
}

void render_fluid(Fluid2D *f)
{
	const Fluxel *fluid_data = f->get_fluid_array();
	int w = f->get_x_dim();
	int h = f->get_y_dim();

	//fill the texture from the fluid data
}

int main(int argc, char **argv)
{
	init_sdl();
	init_gl();
	Fluid2D *my_fluid = new Fluid2D(256, 256);

	while(true)
	{
		float dt = 0.1f;
		my_fluid->simulate(dt);
		render_fluid(my_fluid);
	}

	delete my_fluid;

	quit_app();
	return 0;
}