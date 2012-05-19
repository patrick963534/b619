#ifndef __MZ_TEXTURE_H__
#define __MZ_TEXTURE_H__

#include <mz/defs.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <mz/object.h>
#include <mz/defs.h>

typedef struct mz_image_t
{
    extends_object();

    char*   filename;
    void*   pixels;
    int     format;
    int     w;
    int     h;
    int     real_w;
    int     real_h;
    int     bytes_per_pixel;
    int     pitch;

} mz_image_t;

MZ_API void         mz_image_make_fit_size_power_of_2(mz_image_t *img);
MZ_API mz_image_t*  mz_image_load(const char* filepath);
MZ_API void         mz_image_destruct(mz_object_t* self_);

#endif
