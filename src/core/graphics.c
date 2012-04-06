#include <SDL/SDL.h>
#include <GL/glew.h>
#include "texture.h"

GLuint texture;

void clear()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void flush()
{
	SDL_GL_SwapBuffers();
}

void draw_texture(TEXTURE_ID id)
{
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex3i(0, 0, 0);
		glTexCoord2i(1, 0); glVertex3i(256, 0, 0);
		glTexCoord2i(1, 1); glVertex3i(256, 256, 0);
		glTexCoord2i(0, 1); glVertex3i(0, 256, 0);
	glEnd();
}
