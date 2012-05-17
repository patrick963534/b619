#ifndef __MZ_ANIMATION_H__
#define __MZ_ANIMATION_H__

#include <mz/defs.h>

typedef struct mz_frame_t
{
    int     x;
    int     y;
    int     w;
    int     h;

    float   duration;

} mz_frame_t;

typedef struct mz_sequence_t
{
    char*       name;

    mz_frame_t  *cur_frame;
    mz_frame_t  **frames;
    int         nframe;

} mz_sequence_t;

typedef struct mz_animation_t
{
    mz_sequence_t   *cur_sequence;

    mz_sequence_t   **sequences;
    int             nsequence;

} mz_animation_t;

MZ_API mz_animation_t* mz_animation_load(const char *file);

#endif
