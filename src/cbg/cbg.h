#ifndef __MZ_CBG_H__
#define __MZ_CBG_H__

#include <mz/defs.h>
#include <mz/image.h>

void generate_cbg(const char **image_files, int count, const char *path);

void cbg_to_images(mz_image_t *ret_images, int *ret_count);

#endif
