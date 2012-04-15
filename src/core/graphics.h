#ifndef __MZ_GRAPHICS_H__
#define __MZ_GRAPHICS_H__

void mz_graphics_init();
void mz_graphics_clear();
void mz_graphics_draw_texture(TEXTURE_ID id, int x, int y, int w, int h);
void mz_graphics_flush();

#endif
