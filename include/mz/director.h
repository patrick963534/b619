#ifndef __MZ_DIRECTOR_H__
#define __MZ_DIRECTOR_H__

#include <mz/defs.h>
#include <mz/scene.h>

MZ_API void mz_director_loop();
MZ_API void mz_director_push(mz_scene_t *scene);

#endif
