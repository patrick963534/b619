#ifndef __MZ_NODE_H__
#define __MZ_NODE_H__

#include <mz/object.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/event.h>
#include <mz/container.h>

typedef struct mz_node_t mz_node_t;

typedef int (*mz_event_handler)(mz_node_t *self_, mz_event_t *e);
typedef void (*mz_step_handler)(mz_node_t *self_, int ellapse);
typedef void (*mz_draw_handler)(mz_node_t *self_);

#define extends_node()  \
    extends_container();        \
    mz_node_t       *parent

struct mz_node_t
{
    extends_node();
};

#define extends_node_vtable()   \
    extends_vtable();           \
    mz_draw_handler    draw;          \
    mz_event_handler   event;        \
    mz_step_handler    step

MZ_API mz_vtable_t* mz_node_get_vtable();

MZ_API void mz_node_vtable_draw(mz_node_t *self);

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent);

MZ_API void mz_node_add_child(mz_node_t *node, mz_node_t *child);

#endif
