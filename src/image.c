#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <mz/defs.h>
#include <mz/list.h>
#include <mz/libs.h>
#include <mz/image.h>
#include <mz/object.h>

static int is_power_of_2(int v)
{
    return (((v) & ((v) - 1)) != 0);
}

static SDL_Surface* load_image_with_alpha(const char *path)
{
	//SDL_Surface *temp = IMG_Load(path);
	//SDL_Surface *val = SDL_DisplayFormatAlpha(temp);
	//SDL_FreeSurface(temp);
	//return val;
    return IMG_Load(path);
}

void mz_image_destruct(mz_object_t* self_)
{
    mz_downcast(mz_image_t);

    mz_free(self->filename);
    mz_free(self->pixels);

    mz_object_destruct(self_);
}

mz_image_t* mz_image_load(const char *path)
{
    mz_image_t *image = mz_malloc(sizeof(*image));
	SDL_Surface *surface = load_image_with_alpha(path); 

    if (is_power_of_2(surface->w) || is_power_of_2(surface->h)) 
		goto error;

	if(surface->format->BytesPerPixel == 4)
		image->format = (surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	else if(surface->format->BytesPerPixel == 3)
		image->format = (surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	else
		goto error;

    image->destruct = mz_image_destruct;
    image->filename = mz_strdup(path);
    image->w = surface->w;
    image->h = surface->h;
    image->bytes_per_pixel = surface->format->BytesPerPixel;

    image->pixel_bytes_count = image->bytes_per_pixel * image->w * image->h;
    image->pixels = mz_malloc(image->pixel_bytes_count);
    mz_memcpy(image->pixels, surface->pixels, image->pixel_bytes_count);

    SDL_FreeSurface(surface);

    return image;

error:
    if (surface)
        SDL_FreeSurface(surface);

    logI("load image %s wrong.", path);

    return 0;
}
