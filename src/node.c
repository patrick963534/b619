#include <mz/node.h>
#include <mz/object.h>
#include <mz/container.h>
#include <mz/libs.h>

typedef struct node_vtable_t
{
    extends_node_vtable();
} node_vtable_t;

static void draw(mz_node_t *self_)
{
    mz_unused(self_);
}

MZ_API mz_vtable_t* mz_node_get_vtable()
{
    static node_vtable_t vtable;
    if (vtable.size == 0)
    {
        vtable.draw = draw;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}

MZ_API void mz_node_vtable_draw(mz_node_t *self)
{
    node_vtable_t *vtable = (node_vtable_t*)self->vtable;

    vtable->draw(self);
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
