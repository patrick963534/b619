#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <mz/defs.h>
#include <mz/list.h>
#include <mz/libs.h>

#define IS_POWER_OF_2(v) (((v) & ((v) - 1)) != 0)

typedef struct texture_data_t
{
    list_t          element_;

    const char*     filename;
    SDL_Surface*    surface;
    TEXTURE_ID      id;
    int             format;
} texture_data_t;

static TEXTURE_ID texture_id_counter;
static texture_data_t* temp_texture;

static texture_data_t* get_texture_manager()
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

SDL_Surface *mz_texture_bind_graphics(TEXTURE_ID id)
{
    texture_data_t *manager = get_texture_manager();
    texture_data_t *texture;

    list_for_each_entry(texture, &manager->element_, texture_data_t, element_)
    {
        if (texture->id == id)
        {
            logI("Found Texture Id: %d", id);
            return texture->surface;
        }
    }

    return NULL;
}

void mz_texture_delete(TEXTURE_ID id)
{

}

TEXTURE_ID mz_texture_load(const char *path)
{
    texture_data_t *texture = mz_malloc(sizeof(*texture));
	SDL_Surface *surface = load_image_with_alpha(path); 

    if (IS_POWER_OF_2(surface->w) || IS_POWER_OF_2(surface->h)) 
		goto error;

	if(surface->format->BytesPerPixel == 4)
		texture->format = (surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	else if(surface->format->BytesPerPixel == 3)
		texture->format = (surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	else
		goto error;

    texture->id = texture_id_counter++;
    texture->surface = surface;
    texture->filename = mz_strdup(path);

    INIT_LIST_HEAD(&texture->element_);
    list_move_tail(&texture->element_, &get_texture_manager()->element_);

    temp_texture = texture;

    return texture->id;

error:
    if (surface)
        SDL_FreeSurface(surface);

    logI("load image %s wrong.", path);

    return 0;
}
