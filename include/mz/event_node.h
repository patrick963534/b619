#ifndef __MZ_EVENT_NODE_H__
#define __MZ_EVENT_NODE_H__

#include <mz/node.h>

#define extends_event_node()    \
    extends_node()

typedef struct mz_event_node_t
{
    extends_event_node();
    mz_draw_handler draw;
    mz_event_handler event;
    mz_step_handler step;
} mz_event_node_t;

#define extends_event_node_vtable() \
    extends_node_vtable()

MZ_API mz_vtable_t* mz_event_node_get_vtable();

MZ_API mz_event_node_t* mz_event_node_new(size_t size, mz_node_t *parent);

#endif
