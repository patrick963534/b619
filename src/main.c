#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

SDL_Event ev;



int main(int argc, char** argv)
{
	(void)(argc);(void)(argv);

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *display = SDL_SetVideoMode(640, 480, 0, 0);

	while (1){
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				goto quit;
			}
		}
	}

quit:
	SDL_Quit();
	return 0;
}


