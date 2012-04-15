#include <mz/container.h>
#include <mz/object.h>
#include <mz/list.h>
#include <mz/defs.h>
#include <mz/libs.h>

typedef struct container_vtable_t
{
    extends_vtable();
} container_vtable_t;

static void destruct(mz_object_t *self_)
{
    mz_downcast(mz_container_t);
    mz_object_t *sub_node;

    list_for_each_entry(sub_node, &self->children, mz_object_t, element)
    {
        mz_object_delete(sub_node);
    }

    self->vtable->base_type_vtable->destruct(self_);
}

MZ_API mz_vtable_t* mz_container_get_vtable()
{
    static container_vtable_t vtable;
    if (vtable.size == 0)
    {
        vtable.base_type_vtable = mz_object_get_vtable();
        mz_memcpy(&vtable, vtable.base_type_vtable, vtable.base_type_vtable->size);

        vtable.destruct = destruct;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}
MZ_API mz_container_t* mz_container_new(size_t size)
{
    mz_container_t *v = (mz_container_t*)mz_object_new(size);

    INIT_LIST_HEAD(&v->children);

    return v;
}

MZ_API void mz_container_add(mz_container_t *self, mz_object_t *object)
{
    list_move_tail(&object->element, &self->children);
}

MZ_API void mz_container_remove(mz_object_t *object)
{
    list_del(&object->element);
}
