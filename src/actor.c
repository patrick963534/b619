#include <mz/actor.h>
#include "core/texture.h"
#include "core/graphics.h"

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent)
{
    mz_actor_t *v = (mz_actor_t*)mz_node_new(size, parent);
    //mz_texture_load(file);

    return v;
}
