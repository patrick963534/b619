#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <mz/defs.h>
#include <mz/list.h>
#include <mz/libs.h>

#define IS_POWER_OF_2(v) (((w & (w - 1)) != 0)

typedef struct texture_data_t
{
    list_t          element_;

    const char*     filename;
    SDL_Surface*    surface;
    TEXTURE_ID      id;
    int             format;
}

static TEXTURE_ID texture_id_counter = 0xff;

static texture_manager* get_texture_manager()
{
    static texture_data_t texture_manager;

    if (!texture_manager.id) {
        INIT_LIST_HEAD(&texture_manager.element_);
        texture_manager.id = 0xffffffff;
    }

    return &texture_manager;
}

static SDL_Surface* load_image_with_alpha(const char *path)
{
	//SDL_Surface *temp = IMG_Load(path);
	//SDL_Surface *val = SDL_DisplayFormatAlpha(temp);
	//SDL_FreeSurface(temp);
	//return val;
    return IMG_Load(path);
}

static int load_image_to_gl(const char *path)
{
    texture_data_t *texture = mz_malloc(sizeof(*texture));
	SDL_Surface *surface = load_image_with_alpha(path); //"/home/patrick/image/1.png");

    if (IS_POWER_OF_2(surface->w) || IS_POWER_OF_2(surface->h)) 
		goto error;

	if(surface->format->BytesPerPixel==4) //contains an alpha channel 
		texture->format = (surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	else if(surface->format->BytesPerPixel==3) //no alpha channel 
		texture-format = (surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	else
		goto error;

    texture->id = texture_id_counter++;
    texture->surface = surface;
    texture->filename = mz_strdup(path);

    list_move_tail(&get_texture_manager()->element_, &texture->element_);

    return 0;

error:
    if (image_surface)
        SDL_FreeSurface(image_surface);

    logI("load image %s wrong.", path);

    return 1;
}

void mz_texture_bind_graphics(TEXTURE_ID id)
{
    GLenum texture = 0;
    SDL_Surface *surface = NULL;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, 
                                surface->h, 0, texture_format, 
                                GL_UNSIGNED_BYTE, image_surface->pixels);
}

void mz_texture_delete(TEXTURE_ID id)
{

}

TEXTURE_ID mz_texture_load(const char *filepath)
{
    load_image_to_gl();
}
