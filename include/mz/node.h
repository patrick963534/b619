#ifndef __MZ_NODE_H__
#define __MZ_NODE_H__

#include <mz/object.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/event.h>
#include <mz/container.h>

typedef struct mz_node_t mz_node_t;

#define extends_node()  \
    extends_container();        \
    int         x;          \
    int         y;          \
    int         (*on)(mz_node_t *self_, mz_event_t *e);     \
    void        (*draw)(mz_node_t *self_);                    \
    void        (*step)(mz_node_t *self_, int ellapse);       \
    mz_node_t   *parent

struct mz_node_t
{
    extends_node();
};

MZ_API void mz_node_draw(mz_node_t *node);

MZ_API int mz_node_on(mz_node_t *node, mz_event_t *e);

MZ_API void mz_node_step(mz_node_t *node, int ellapse);

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent);

MZ_API void mz_node_add_child(mz_node_t *node, mz_node_t *child);

#endif
