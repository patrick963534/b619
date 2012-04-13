#ifndef __MZ_TEXTURE_H__
#define __MZ_TEXTURE_H__

#include <mz/defs.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

TEXTURE_ID mz_texture_load(const char* filepath);
void mz_texture_delete(TEXTURE_ID id);

SDL_Surface *mz_texture_bind_graphics(TEXTURE_ID id);

#endif
