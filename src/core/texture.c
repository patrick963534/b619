#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

SDL_Surface *image_surface;
GLenum texture;
int texture_format;

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
