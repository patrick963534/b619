#include <mz/scene.h>
#include <mz/node.h>
#include <mz/actor.h>
#include <mz/event.h>

typedef struct scene_vtable_t
{
    extends_scene_vtable();
} scene_vtable_t;

static void draw(mz_node_t *self_)
{
    mz_downcast(mz_scene_t);
    mz_actor_t *actor;

    list_for_each_entry(actor, &self->element_, mz_actor_t, element_) 
    {
        mz_node_vtable_draw((mz_node_t*)actor);
    }
}

MZ_API mz_vtable_t* mz_scene_get_vtable()
{
    static scene_vtable_t vtable;

    if (vtable.size == 0)
    {
        mz_vtable_t *base_vtable = mz_get_node_vtable();
        mz_memcpy(&vtable, base_vtable, base_vtable->size);

        vtable.draw = draw;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent)
{
    mz_scene_t *v = (mz_scene_t*)mz_node_new(size, parent);
    v->vtable = mz_scene_get_vtable();

    return v;
}
