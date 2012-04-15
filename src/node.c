#include <mz/node.h>
#include <mz/object.h>
#include <mz/container.h>
#include <mz/libs.h>
#include <mz/event.h>

typedef struct node_vtable_t
{
    extends_node_vtable();
} node_vtable_t;

static void destruct(mz_object_t *self_)
{
    mz_downcast(mz_node_t);



    self->vtable->base_type_vtable->destruct(self_);
}

static int event_on(mz_node_t *self_, mz_event_t *e)
{
    mz_unused(self_);
    mz_unused(e);

    return 0;
}

static void step(mz_node_t *self_, int ellapse)
{
    mz_unused(self_);
    mz_unused(ellapse);
}

static void draw(mz_node_t *self_)
{
    mz_unused(self_);
}

MZ_API mz_vtable_t* mz_node_get_vtable()
{
    static node_vtable_t vtable;
    if (vtable.size == 0)
    {
        vtable.base_type_vtable = mz_object_get_vtable();
        mz_memcpy(&vtable, vtable.base_type_vtable, vtable.base_type_vtable->size);

        vtable.draw = draw;
        vtable.step = step;
        vtable.event = event_on;
        vtable.destruct = destruct;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}

MZ_API void mz_node_vtable_draw(mz_node_t *self)
{
    if (self->vtable->size >= mz_node_get_vtable()->size)
    {
        node_vtable_t *vtable = (node_vtable_t*)self->vtable;
        vtable->draw(self);
    }
}

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent)
{
    mz_node_t *v = (mz_node_t*)mz_container_new(size);
    v->vtable = mz_node_get_vtable();

    if (parent)
        mz_node_add_child(parent, v);

    return v;
}

MZ_API void mz_node_add_child(mz_node_t *self, mz_node_t *child)
{
    mz_container_add((mz_container_t*)self, (mz_object_t*)child);
    child->parent = self;
}
