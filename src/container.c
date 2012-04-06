#include <mz/container.h>
#include <mz/object.h>
#include <mz/list.h>
#include <mz/defs.h>
#include <mz/libs.h>

MZ_API mz_container_t* mz_container_new(size_t size)
{
    mz_container_t *v = (mz_container_t*)mz_malloc(size);

    INIT_LIST_HEAD(&v->element_);

    return v;
}

MZ_API void mz_container_add(mz_container_t *self, mz_object_t *object)
{
    list_move_tail(&self->element_, &object->element_);
}
