#include <mz/animation.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include "ani/ani_xml_file.h"

typedef struct ani_info_t
{
    const char  **images;
    int         image_count;
} ani_info_t;

static int is_in_array(const char **names, int count, const char *target)
{
    int i;

    for (i = 0; i < count; i++)
        if (mz_strequal(names[i], target))
            return 1;

    return 0;
}

static void get_sequence_name_list(animation_set_tag_t *set, const char ***ret_names, int *ret_count)
{
    const char **names = mz_malloc(sizeof(names[0]) * set->nanimation);
    int count = 0;
    int i;
    
    for (i = 0; i < set->nanimation;  i++) {
        if (is_in_array(names, count, set->animations[i]->name))
            continue;

        names[count++] = mz_strdup(set->animations[i]->name);
    }

    names = mz_realloc(names, sizeof(names[0]) * count);

    *ret_names = names;
    *ret_count = count;
}

static void get_unique_image_names_in_one_animation(animation_set_tag_t *set, const char *anim_name, const char ***ret_images, int *ret_image_count)
{
    int nimage = 4;
    const char **images = mz_malloc(sizeof(images[0]) * nimage);
    int image_count = 0;

    int i, j, k, h;

    for (i = 0; i < set->nanimation; i++) {
        animation_tag_t *anim = set->animations[i];

        if (!mz_strequal(anim->name, anim_name))
            continue;

        for (j = 0; j < anim->nframe; j++) {
            frame_tag_t *frame = anim->frames[j];

            for (k = 0; k < frame->nimage; k++) {
                image_tag_t *img = frame->images[k];
                
                if (is_in_array(images, image_count, img->filepath))
                    continue;

                images[image_count++] = mz_strdup(img->filepath);

                if (image_count == nimage) {
                    nimage += nimage;
                    images = mz_realloc(images, sizeof(images[0]) * nimage);
                }
            }
        }     
    }

    *ret_images = images;
    *ret_image_count = image_count;
}

static int get_array_id(const char **names, int count, const char *target)
{
    int i;

    for (i = 0; i < count; i++)
        if (mz_strequal(names[i], target))
            return i;

    return -1;
}

static mz_frame_t* generate_one_ani_frame(frame_tag_t *frame_tag, ani_info_t *info)
{
    int nitem = 4;
    int item_id = 0;
    int i;

    mz_frame_t *frame = mz_malloc(sizeof(*frame));
    frame->patterns = mz_malloc(sizeof(frame->patterns[0]) * nitem);

    for (i = 0; i < frame_tag->nimage; i++) { 
        image_tag_t *image_tag = frame_tag->images[i];
        mz_pattern_t *p = mz_malloc(sizeof(*p));

        p->origin_x = image_tag->origin_x;
        p->origin_y = image_tag->origin_y;
        p->image_id = get_array_id(info->images, info->image_count, image_tag->filepath);
        
        frame->patterns[item_id++] = p;
        if (item_id == nitem) {
            nitem += nitem;
            frame->patterns = mz_realloc(frame->patterns, sizeof(frame->patterns[0]) * nitem);
        }
    }

    frame->duration = frame_tag->duration;
    frame->npattern = frame_tag->nimage;
}

static mz_sequence_t* generate_one_ani_sequence(animation_tag_t *ani_tag, ani_info_t *ani_info)
{
    int nitem = 4;
    int item_id = 0;
    int i;

    mz_sequence_t *sequence = mz_malloc(sizeof(*sequence));
    sequence->frames = mz_malloc(sizeof(sequence->frames[0]) * nitem);

    for (i = 0; i < ani_tag->nframe; i++) { 
        frame_tag_t *f_tag = ani_tag->frames[i];

        sequence->frames[item_id++] = generate_one_ani_frame(f_tag, ani_info);

        if (item_id == nitem) {
            nitem += nitem;
            sequence->frames = mz_realloc(sequence->frames, sizeof(sequence->frames) * nitem);
        }
    }

    if (ani_tag->stat)
        sequence->stat = mz_strdup(ani_tag->stat);
    else
        sequence->stat = mz_strdup("");

    sequence->nframe = ani_tag->nframe;
};

static void write_to_ani_file(mz_animation_t *animation, const char *folder, ani_info_t *info)
{
    FILE *fp;

    char fullpath[256];
    char filename[256];

    mz_snprintf(filename, sizeof(filename), "%s.ani", animation->name);
    mz_path_combine_path(fullpath, sizeof(fullpath), folder, filename);   

    fp = mz_fopen(fullpath, "w");

    {
        int i, j, k;
        char buf[256];

        mz_file_write_int(fp, info->image_count);
        for (i = 0; i < info->image_count; i++)
            mz_file_write_string(fp, info->images[i]);

        mz_file_write_string(fp, animation->name);
        mz_file_write_int(fp, animation->nsequence);

        for (i = 0; i < animation->nsequence; i++) {
            mz_sequence_t *seq = animation->sequences[i];

            mz_file_write_string(fp, seq->stat);
            mz_file_write_int(fp, seq->nframe);

            for (j = 0; j < seq->nframe; j++) {
                mz_frame_t *frame = seq->frames[j];

                mz_file_write_int(fp, frame->duration);
                mz_file_write_int(fp, frame->npattern);

                for (k = 0; k < frame->npattern; k++) {
                    mz_pattern_t *pattern = frame->patterns[k];

                    mz_file_write_int(fp, pattern->origin_x);
                    mz_file_write_int(fp, pattern->origin_y);
                    mz_file_write_int(fp, pattern->image_id);
                }
            }
        }
    }

    mz_fclose(fp);
}

MZ_API mz_animation_t* mz_animation_load(const char *ani_file)
{
    mz_animation_t *ani;
    FILE *fp;
    int i, j, k;
    ani_info_t info;
    char buf[512];

    assert(ani_file);
    assert(mz_str_is_end_with(ani_file, ".ani"));

    ani = mz_malloc(sizeof(*ani));

    fp = mz_fopen(ani_file, "r");

    info.image_count = mz_file_read_int(fp);
    info.images = mz_malloc(sizeof(info.images[0]) * info.image_count);

    for (i = 0; i < info.image_count; i++)
        info.images[i] = mz_file_read_string(fp);

    ani->nimage = info.image_count;
    ani->images = mz_malloc(sizeof(ani->images[0]) * ani->nimage);
    for (i = 0; i < info.image_count; i++) {
        char folder[512];
        mz_path_get_folder(folder, sizeof(folder), ani_file);
        mz_path_combine_path(buf, sizeof(buf), folder, info.images[i]);
        ani->images[i] = mz_image_load(buf);
    }

    ani->name = mz_file_read_string(fp);
    ani->nsequence = mz_file_read_int(fp);
    ani->sequences = mz_malloc(sizeof(ani->sequences[0]) * ani->nsequence);

    for (i = 0; i < ani->nsequence; i++) {
        mz_sequence_t *seq = mz_malloc(sizeof(*seq));
        seq->stat = mz_file_read_string(fp);
        seq->nframe = mz_file_read_int(fp);
        seq->frames = mz_malloc(sizeof(seq->frames[0]) * seq->nframe);
        ani->sequences[i] = seq;

        for (j = 0; j < seq->nframe; j++) {
            mz_frame_t *frame = mz_malloc(sizeof(*frame));
            frame->duration = mz_file_read_int(fp);
            frame->npattern = mz_file_read_int(fp);
            frame->patterns = mz_malloc(sizeof(frame->patterns[0]) * frame->npattern);
            seq->frames[j] = frame;

            for (k = 0; k < frame->npattern; k++) {
                mz_pattern_t *pattern = mz_malloc(sizeof(*pattern));
                pattern->origin_x = mz_file_read_int(fp);
                pattern->origin_y = mz_file_read_int(fp);
                pattern->image_id = mz_file_read_int(fp);
                frame->patterns[k] = pattern;
            }
        }

    }

    ani->cur_sequence_id = 0;

    mz_fclose(fp);

    return ani;
}

static void generate_one_ani_file(animation_set_tag_t *set, const char *anim_name, const char *folder)
{
    ani_info_t ani_info = {0};
    int nitem = 4;
    int item_id = 0;
    int i;

    mz_animation_t *animation = mz_malloc(sizeof(*animation));
    animation->sequences = mz_malloc(sizeof(animation->sequences[0]) * nitem);

    get_unique_image_names_in_one_animation(set, anim_name, &ani_info.images, &ani_info.image_count);

    for (i = 0; i < set->nanimation; i++) {
        animation_tag_t *ani_tag = set->animations[i];
        mz_sequence_t *sequence;

        if (!mz_strequal(ani_tag->name, anim_name))
            continue;

        sequence = generate_one_ani_sequence(ani_tag, &ani_info);
        animation->sequences[item_id++] = sequence;
        
        if (item_id == nitem) {
            nitem += nitem;
            animation->sequences = mz_realloc(animation->sequences, sizeof(animation->sequences[0]) * nitem);
        }
    }

    animation->name = mz_strdup(anim_name);
    animation->nsequence = item_id;

    write_to_ani_file(animation, folder, &ani_info);
}

MZ_API int mz_animation_generate_ani_file(const char *xml_file, const char *dst_folder)
{
    animation_set_tag_t *set = parse_xml(xml_file);
    const char **animation_names;
    int animation_count;
    int i;

    get_sequence_name_list(set, &animation_names, &animation_count);

    for (i = 0; i < animation_count; i++) 
        generate_one_ani_file(set, animation_names[i], dst_folder);

    return 0;
}
