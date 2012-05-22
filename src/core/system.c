#include "system.h"
#include <SDL/SDL.h>
#include <mz/event.h>
#include <mz/libs.h>
#include "graphics.h"
#include "util.c.h"

static mz_system_t _system;

static int get_timer_event(mz_event_t *event)
{
    return 0;
}

static int get_update_event(mz_event_t *event)
{
    static Uint32 time = 0;

    if (time == 0)
        time =SDL_GetTicks();

    Uint32 new_time = SDL_GetTicks();

    if (new_time - time > 20) {
        event->ellapse = new_time - time;
        event->type = mz.events.UpdateFrame;
        time = new_time;
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

int mz_system_get_ellapse()
{
    return 0;
}

void mz_system_init()
{
    mz_graphics_init();

	_system.wait_event = wait_event;
}

void mz_system_quit()
{

}

mz_system_t *mz_system_instance()
{
	return &_system;
}
