#ifndef __MZ_SCENE_H__
#define __MZ_SCENE_H__

#include <mz/object.h>
#include <mz/list.h>
#include <mz/node.h>
#include <mz/event.h>
#include <mz/container.h>

#define extends_scene()     \
    extends_node()

typedef struct mz_scene_t
{
    extends_scene();

} mz_scene_t;

#define extends_scene_vtable()  \
    extends_node_vtable()       

typedef void (*on_handler_)(mz_node_t *self_, mz_event_t *e) on_handler;

MZ_API void mz_scene_add_on_handler(mz_scene_t *self_, hz_node_t *target, on_handler on); 

MZ_API mz_vtable_t* mz_scene_get_vtable();

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent);

#endif
