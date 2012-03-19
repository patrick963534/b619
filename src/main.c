#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

static SDL_Surface *display_surface;
static SDL_Surface *image_surface;

static GLuint texture;
static GLenum texture_format;

static Uint32 old_time;
static SDL_Event ev;

static void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	if((display_surface = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
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

static SDL_Surface *load_image_with_alpha(const char* path)
{
	SDL_Surface *temp = IMG_Load(path);
	SDL_Surface *val = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
	return val;
}

static void load_image_to_gl()
{
	image_surface = load_image_with_alpha("/home/patrick/image/1.png");
	if (image_surface)
		printf("get image surface.\n");

	if ((image_surface->w & (image_surface->w - 1)) != 0 || (image_surface->h & (image_surface->h - 1)) != 0){
		printf("warning: the surface width/height is not a power of 2. \r\n");
		return;
	}

	if(image_surface->format->BytesPerPixel==4) //contains an alpha channel
	{
		texture_format = (image_surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	}
	else if(image_surface->format->BytesPerPixel==3) //no alpha channel
	{
		texture_format = (image_surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	}
	else
	{
		printf("wrong.");
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, image_surface->format->BytesPerPixel, 
		    image_surface->w, image_surface->h, 0, 
		    texture_format, GL_UNSIGNED_BYTE, image_surface->pixels);
}

static void render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	SDL_GL_SwapBuffers();
}

int main(int argc, char** argv)
{
	(void)(argc);
	(void)(argv);

	init();
	load_image_to_gl();


	while (1) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT: goto quit;
			}
		}

		render();
		usleep(300 * 1000);
	}

quit:
	SDL_Quit();
	return 0;
}
