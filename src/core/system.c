#include "system.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <mz/event.h>
#include <mz/libs.h>
#include "util.c.h"

static mz_system_t _system;
static SDL_Surface *display_surface;

static int get_timer_event(mz_event_t *event)
{
    return 0;
}

static int get_update_event(mz_event_t *event)
{
    static Uint32 time = 0;
    Uint32 new_time = SDL_GetTicks();

    if (new_time - time > 300) {
        time = new_time;
        event->type = mz.events.UpdateFrame;
        return 1;
    }

	return 0;
}

static int get_sdl_event(mz_event_t *event)
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		event->type = e.type;
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			event->keyboard.keycode = e.key.keysym.sym;
		else if (e.type == SDL_QUIT)
			event->type = e.type;
	
		return 1;
	}
	return 0;
}

static void wait_event(mz_event_t *event)
{
	while (1) {
		if (get_update_event(event))
			return;
		
		if (get_sdl_event(event))
			return;

        if (get_timer_event(event))
            return;

		mz_sleep(32);
	}
}

static void sdl_opengl_init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

    if((display_surface = SDL_SetVideoMode(640, 480, 32, 
        SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
            return;
    }

    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
}

void mz_system_init()
{
	sdl_opengl_init();

	_system.wait_event = wait_event;
}

void mz_system_quit()
{

}

mz_system_t *mz_system_instance()
{
	return &_system;
}
