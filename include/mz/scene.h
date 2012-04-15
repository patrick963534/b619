#ifndef __MZ_SCENE_H__
#define __MZ_SCENE_H__

#include <mz/object.h>
#include <mz/list.h>
#include <mz/node.h>
#include <mz/event.h>
#include <mz/container.h>

#define extends_scene()     \
    extends_node();         \
    list_t event_handler_element;   \
    list_t step_handler_element

typedef struct mz_scene_t
{
    extends_scene();

} mz_scene_t;

#define extends_scene_vtable()  \
    extends_node_vtable()       \

MZ_API void mz_scene_process_event(mz_scene_t *self);

MZ_API void mz_scene_process_event(mz_scene_t *self);

MZ_API mz_vtable_t* mz_scene_get_vtable();

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent);

#endif
