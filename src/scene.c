#include <mz/scene.h>
#include <mz/actor.h>

MZ_API mz_scene_t* mz_scene_new(size_t size, mz_node_t *parent)
{
    mz_scene_t *v = (mz_scene_t*)mz_node_new(size, parent);

    return v;
}
