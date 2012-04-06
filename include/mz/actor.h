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

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent);

#endif
