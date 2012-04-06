#ifndef __MZ_SCENE_H__
#define __MZ_SCENE_H__

#include <mz/object.h>
#include <mz/list.h>

typedef struct mz_scene_t
{
    mz_object_t object;

    list_t      children;

} mz_scene_t;

#endif
