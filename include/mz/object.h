#ifndef __MZ_OBJECT_H__
#define __MZ_OBJECT_H__

#include <mz/list.h>
#include <mz/libs.h>

typedef struct mz_vtable_t mz_vtable_t;

#define extends_vtable()    \
    int size;               \
    mz_vtable_t     *base_type_vtable;   \
    void            (*destruct)()   

struct mz_vtable_t
{
    extends_vtable();
};

#define extends_object()    \
    int     is_in_heap_;    \
    int     is_destructed_; \
    mz_vtable_t     *vtable;    \
    list_t  element

typedef struct mz_object_t
{
    extends_object();
} mz_object_t;

MZ_API mz_vtable_t* mz_object_get_vtable();

MZ_API void mz_object_delete(mz_object_t *obj);

MZ_API mz_object_t* mz_object_new(size_t size);

#endif
