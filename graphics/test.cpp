#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>
#include <assert.h>

#include "texture.h"

const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;
    
SDL_Window* sdl_window = NULL;
SDL_GLContext sdl_gl_context = NULL;
Texture *t = NULL;



void quit_app()
{
    SDL_GL_DeleteContext(sdl_gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    if(t) delete t;
    exit(0);
}

void init_sdl()
{
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        sdl_window = SDL_CreateWindow("Graphics Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        assert(sdl_window);

        //sdl_screen_surface = SDL_GetWindowSurface(window);
        
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

void game_loop()
{
	Uint32 ticks = SDL_GetTicks();
    float game_time = (float)ticks;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    t->render_gl();
    glBegin(GL_QUADS);
    	glColor3f(1.0f, 1.0f, 1.0f);
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(0.0f, 0.0f, 0.0f);
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(1.0f, 0.0f, 0.0f);
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(1.0f, 1.0f, 0.0f);
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

	glFlush();
	SDL_GL_SwapWindow(sdl_window);
}

int main(int argc, char **argv)
{
	init_sdl();
	init_gl();

	t = new Texture("data/test.tga");
	t->load();

	while(true)
	{
		process_events();
		game_loop();
	}

	return 0;
}