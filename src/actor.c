#include <mz/actor.h>
#include <mz/node.h>
#include "core/texture.h"
#include "core/graphics.h"

typedef struct actor_vtable_t
{
    extends_actor_vtable();
} actor_vtable_t;

static void draw(mz_node_t *self_)
{
    mz_downcast(mz_actor_t);

    mz_graphics_draw_texture(self->texture, self->x, self->y, 512, 512);
}

MZ_API mz_vtable_t* mz_actor_get_vtable()
{
    static actor_vtable_t vtable;

    if (vtable.size == 0)
    {
        mz_vtable_t *base_vtable = mz_node_get_vtable();
        mz_memcpy(&vtable, base_vtable, base_vtable->size);

        vtable.draw = draw;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent)
{
    mz_actor_t *v = (mz_actor_t*)mz_node_new(size, parent);
    v->texture = mz_texture_load(file);
    v->vtable = mz_actor_get_vtable();

    return v;
}
