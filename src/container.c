#include <mz/container.h>
#include <mz/object.h>
#include <mz/list.h>
#include <mz/defs.h>
#include <mz/libs.h>

static void destruct(mz_object_t *self_)
{
    mz_downcast(mz_container_t);
    mz_object_t *sub_node;

    list_for_each_entry(sub_node, &self->children, mz_object_t, element)
    {
        mz_object_delete(sub_node);
    }
}

MZ_API mz_container_t* mz_container_new(size_t size)
{
    mz_container_t *v = (mz_container_t*)mz_object_new(size);

    INIT_LIST_HEAD(&v->children);

    v->destruct = destruct;

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
