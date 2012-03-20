#include <mz/event.h>
#include <mz/defs.h>
#include <SDL/SDL.h>

#define UPDATE_FRAME 128

const mz_constants_t mz = {
	{
		UPDATE_FRAME,
		SDL_KEYDOWN,
		SDL_KEYUP
	},
	{
		SDLK_ESCAPE,
		SDLK_RETURN,
		SDLK_SPACE,

		SDLK_LEFT,
		SDLK_RIGHT,
		SDLK_UP,
		SDLK_DOWN,

		SDLK_BACKSPACE
	}
};
