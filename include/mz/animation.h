#ifndef __MZ_ANIMATION_H__
#define __MZ_ANIMATION_H__

#include <mz/defs.h>
#include <mz/image.h>

typedef struct mz_pattern_t
{
    int     x;
    int     y;
    int     image_id;

} mz_pattern_t;

typedef struct mz_frame_t
{
    mz_pattern_t    **patterns;
    int             npattern;

    int             duration;

} mz_frame_t;

typedef struct mz_sequence_t
{
    char        *stat;
    mz_frame_t  **frames;
    int         nframe;
} mz_sequence_t;

typedef struct mz_animation_t
{
    char            *name;
    mz_sequence_t   **sequences;
    int             nsequence;
} mz_animation_t;

MZ_API int mz_animation_generate_ani_file(const char *xml_file, const char *dst_folder);
//MZ_API mz_animation_t* mz_animation_load(const char *ani_file);

#endif
