#include <mz/object.h>
#include <mz/list.h>

static void destruct(mz_object_t *self_)
{
    mz_unused(self_);
}

MZ_API mz_vtable_t* mz_object_get_vtable()
{
    static mz_vtable_t vtable;

    if (vtable.size == 0)
    {
        vtable.size = sizeof(vtable);
        vtable.destruct = destruct;
    }

    return &vtable;
}

MZ_API mz_object_t* mz_object_new(size_t size)
{
    mz_object_t *v = (mz_object_t*)mz_malloc(size);
    INIT_LIST_HEAD(&v->element);

    return v;
}

MZ_API void mz_object_delete(mz_object_t *obj)
{
    if (!obj)
        return;

    obj->vtable->destruct(obj);

    obj->is_destructed_ = 1;

    if (obj->is_in_heap_)
        mz_free(obj);
}
