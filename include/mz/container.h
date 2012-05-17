#ifndef __MZ_CONTAINER_H__
#define __MZ_CONTAINER_H__

#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/object.h>
#include <mz/list.h>

#define extends_container() \
    extends_object();       \
    list_t      children

typedef struct mz_container_t
{
    extends_container();

} mz_container_t;

MZ_API mz_container_t* mz_container_new(size_t size);

MZ_API void mz_container_add(mz_container_t *container, mz_object_t *object);

MZ_API void mz_container_remove(mz_object_t *object);

MZ_API void destruct(mz_object_t *self_);

#endif
