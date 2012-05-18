#ifndef __MZ_DIRECTOR_H__
#define __MZ_DIRECTOR_H__

#include <mz/defs.h>
#include <mz/scene.h>

#define MAX_SCENE_COUNT 32

typedef struct mz_director_t
{
    mz_scene_t* scene_stack[MAX_SCENE_COUNT];
    int         cur_scene_id;
    mz_scene_t* cur_scene;
    mz_scene_t* next_scene;
} mz_director_t;

MZ_API mz_scene_t* mz_main();

MZ_API void mz_director_push(mz_scene_t *scene);

MZ_API mz_director_t* mz_director_new(size_t size, mz_scene_t* scene);

#endif
