#ifndef __MZ_ACTOR_H__
#define __MZ_ACTOR_H__

#include <mz/object.h>
#include <mz/node.h>
#include <mz/defs.h>

typedef struct mz_actor_t_ mz_actor_t;

#define extends_actor() \
    extends_node(); \
    TEXTURE_ID  texture

struct mz_actor_t_
{
    extends_actor();
};

#define extends_actor_vtable()  \
    extends_node_vtable()       

MZ_API mz_vtable_t* mz_actor_get_vtable();

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent);

#endif
