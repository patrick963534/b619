#ifndef __MZ_ACTOR_H__
#define __MZ_ACTOR_H__

#include <mz/object.h>
#include <mz/node.h>
#include <mz/defs.h>
#include <mz/image.h>
#include <mz/animation.h>

typedef struct mz_actor_t mz_actor_t;

#define extends_actor() \
    extends_node();             \
    mz_animation_t *animation;  \
    mz_image_t *image

struct mz_actor_t
{
    extends_actor();
};

MZ_API mz_actor_t* mz_actor_new(const char *file, size_t size, mz_node_t *parent);

MZ_API void mz_actor_destruct(mz_object_t* self_);

#endif
