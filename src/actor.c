#include <mz/actor.h>
#include <mz/node.h>
#include "core/texture.h"
#include "core/graphics.h"

static void draw(mz_node_t *self_)
{
    mz_downcast(mz_actor_t);

    mz_graphics_draw_texture(self->texture, self->x, self->y, 512, 512);
}

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent)
{
    mz_actor_t *v = (mz_actor_t*)mz_node_new(size, parent);
    v->texture = mz_texture_load(file);

    v->draw = draw;

    return v;
}
