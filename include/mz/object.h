#ifndef __MZ_OBJECT_H__
#define __MZ_OBJECT_H__

#include <mz/list.h>
#include <mz/libs.h>

#define extends_object()    \
    list_t  element_

typedef struct mz_object_t
{
    extends_object();
} mz_object_t;

#define extends_vtable()    \
    int size

typedef struct mz_vtable_t
{
    extends_vtable();
} mz_vtable_t;

MZ_API mz_vtable_t* mz_get_object_vtable();

#endif
