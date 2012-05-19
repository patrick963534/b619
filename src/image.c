#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <mz/defs.h>
#include <mz/list.h>
#include <mz/libs.h>
#include <mz/image.h>
#include <mz/object.h>

static SDL_Surface* load_image_with_alpha(const char *path)
{
	//SDL_Surface *temp = IMG_Load(path);
	//SDL_Surface *val = SDL_DisplayFormatAlpha(temp);
	//SDL_FreeSurface(temp);
	//return val;
    return IMG_Load(path);
}

MZ_API void mz_image_destruct(mz_object_t* self_)
{
    mz_downcast(mz_image_t);

    mz_free(self->filename);
    mz_free(self->pixels);

    mz_object_destruct(self_);
}

mz_image_t* mz_image_create_empty()
{
    mz_image_t *image = mz_malloc(sizeof(*image));
    return image;
}

MZ_API void mz_image_make_fit_size_power_of_2(mz_image_t *img)
{
    int p2_w = mz_get_larger_power_of_2(img->w);
    int p2_h = mz_get_larger_power_of_2(img->h);
    void *pixels;
    int pitch;
    int i;

    if (p2_w == img->w && p2_h == img->h)
        return;

    pitch = p2_w * img->bytes_per_pixel;
    pixels = mz_malloc(p2_h * pitch);

    for (i = 0; i < img->h; i++)
        mz_memcpy(pixels + i * pitch, img->pixels + i * img->pitch, 
                                    img->w * img->bytes_per_pixel);

    mz_free(img->pixels);

    img->w = p2_w;
    img->h = p2_h;
    img->pitch = pitch;
    img->pixels = pixels;
}

MZ_API mz_image_t* mz_image_load(const char *path)
{
    mz_image_t *image = mz_malloc(sizeof(*image));
	SDL_Surface *surface = load_image_with_alpha(path); 
    int bytes_count;

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
    image->real_w = surface->w;
    image->real_h = surface->h;
    image->bytes_per_pixel = surface->format->BytesPerPixel;
    image->pitch = surface->pitch;

    bytes_count = image->pitch * image->h;
    image->pixels = mz_malloc(bytes_count);
    mz_memcpy(image->pixels, surface->pixels, bytes_count);

    SDL_FreeSurface(surface);

    return image;

error:
    if (surface)
        SDL_FreeSurface(surface);

    logI("load image %s wrong.", path);

    return 0;
}
