#ifndef __MZ_OBJECT_H__
#define __MZ_OBJECT_H__

#include <mz/list.h>
#include <mz/libs.h>

typedef struct mz_object_t mz_object_t;

#define extends_object()    \
    int     is_in_heap_;    \
    int     is_destructed_; \
    void    (*destruct)(mz_object_t* self_);  \
    list_t  element

struct mz_object_t
{
    extends_object();
};

MZ_API void mz_object_delete(mz_object_t *obj);

MZ_API mz_object_t* mz_object_new(size_t size);

MZ_API void mz_object_destruct(mz_object_t *self_);

#endif
