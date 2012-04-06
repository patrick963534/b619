#ifndef __MZ_ACTOR_H__
#define __MZ_ACTOR_H__

#include <mz/object.h>
#include <mz/defs.h>

typedef struct mz_actor_t
{
    mz_object_t object;

    TEXTURE_ID texture;

} mz_actor_t;
