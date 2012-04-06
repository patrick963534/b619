#ifndef __MZ_OBJECT_H__
#define __MZ_OBJECT_H__

#include <mz/list.h>

#define extends_object()    \
    list_t  element_

typedef struct mz_object_t
{
    extends_object();
} mz_object_t;

#endif
