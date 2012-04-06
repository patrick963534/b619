#ifndef __MZ_TEXTURE_H__
#define __MZ_TEXTURE_H__

#include <mz/defs.h>

TEXTURE_ID load_texture(const char* filepath);
void delete_texture(TEXTURE_ID id);

void bind_texture(TEXTURE_ID id);

#endif
