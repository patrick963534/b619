#include <SDL/SDL.h>
#include <GL/glew.h>
#include <mz/libs.h>
#include "texture.h"

static SDL_Surface *display_surface;
static GLuint gl_texture;

void mz_graphics_clear()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mz_graphics_flush()
{
    SDL_GL_SwapBuffers();
}

void mz_graphics_init()
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

    glGenTextures(1, &gl_texture);
}

static GLuint get_pixel_format(SDL_Surface *surface)
{
    GLuint format = 0;

	if(surface->format->BytesPerPixel == 4)
		format = (surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	else if(surface->format->BytesPerPixel == 3)
		format = (surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	else
		logI("wrong image format.");

    return format;
}

void mz_graphics_draw_texture(TEXTURE_ID texture_id)
{
    SDL_Surface *surface = mz_texture_bind_graphics(texture_id);

    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, gl_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, 
                                surface->w, surface->h, 
                                0, get_pixel_format(surface),
                                GL_UNSIGNED_BYTE, surface->pixels);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(0, 0, 0);
        glTexCoord2i(1, 0); glVertex3i(256, 0, 0);
        glTexCoord2i(1, 1); glVertex3i(256, 256, 0);
        glTexCoord2i(0, 1); glVertex3i(0, 256, 0);
    glEnd();
}
