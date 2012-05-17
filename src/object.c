#include <mz/object.h>
#include <mz/list.h>

MZ_API void mz_object_destruct(mz_object_t *self_)
{
    mz_unused(self_);
}

MZ_API mz_object_t* mz_object_new(size_t size)
{
    mz_object_t *v = (mz_object_t*)mz_malloc(size);
    INIT_LIST_HEAD(&v->element);

    v->destruct = mz_object_destruct;

    return v;
}

MZ_API void mz_object_delete(mz_object_t *obj)
{
    if (!obj)
        return;

    if (obj->destruct)
        obj->destruct(obj);

    obj->is_destructed_ = 1;

    if (obj->is_in_heap_)
        mz_free(obj);
}
