#ifndef __MZ_SCENE_H__
#define __MZ_SCENE_H__

#include <mz/object.h>
#include <mz/list.h>
#include <mz/node.h>
#include <mz/container.h>

typedef struct mz_scene_t
{
    extends_container();

} mz_scene_t;

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent);

#endif
