#include <mz/actor.h>
#include <mz/node.h>
#include <mz/image.h>
#include <mz/defs.h>
#include "core/graphics.h"

static void draw(mz_node_t *self_)
{
    mz_downcast(mz_actor_t);

    mz_graphics_draw_texture(self->image, self->x, self->y);
}

MZ_API void mz_actor_destruct(mz_object_t* self_)
{
    mz_node_destruct((mz_node_t*)self_);
}

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent)
{
    mz_actor_t *v = (mz_actor_t*)mz_node_new(size, parent);
    v->image = mz_image_load(file);
    mz_image_make_fit_size_power_of_2(v->image);

    v->draw = draw;
    v->destruct = mz_actor_destruct;

    return v;
}
