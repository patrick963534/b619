#ifndef __MZ_NODE_H__
#define __MZ_NODE_H__

#include <mz/object.h>
#include <mz/libs.h>
#include <mz/defs.h>

typedef struct mz_node_t mz_node_t;

#define extends_node()  \
    extends_object();   \
    mz_node_t *parent

struct mz_node_t
{
    extends_node();
};

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent);

MZ_API void mz_node_add_child(mz_node_t *node, mz_node_t *child);

#endif
