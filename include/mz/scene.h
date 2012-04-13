#ifndef __MZ_SCENE_H__
#define __MZ_SCENE_H__

#include <mz/object.h>
#include <mz/list.h>
#include <mz/node.h>
#include <mz/container.h>

typedef struct mz_scene_t
{
    extends_node();

} mz_scene_t;

#define extends_scene_vtable()  \
    extends_node_vtable()       

MZ_API mz_vtable_t* mz_get_scene_vtable();

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent);

#endif
