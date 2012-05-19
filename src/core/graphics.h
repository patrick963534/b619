#ifndef __MZ_GRAPHICS_H__
#define __MZ_GRAPHICS_H__

#include <mz/image.h>

void mz_graphics_init();
void mz_graphics_clear();
void mz_graphics_draw_texture(mz_image_t* image, int x, int y);
void mz_graphics_flush();

#endif
