#include <mz/animation.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/simple_xml.h>

typedef struct image_tag_t
{
    char    *filepath;
    int     origin_x;
    int     origin_y;
} image_tag_t;

typedef struct frame_tag_t
{
    int             duration;
    image_tag_t     **images;
    int             nimage;

} frame_tag_t;

typedef struct animation_tag_t
{
    char        *name;
    char        *stat;
    frame_tag_t **frames;
    int         nframe;

} animation_tag_t;

typedef struct animation_set_tag_t
{
    animation_tag_t     **animations;
    int                 nanimation;
} animation_set_tag_t;

static void read_image(mz_xml_node_t node, frame_tag_t *frame)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "image";

    int nitem = 4;
    int count = 0;
    image_tag_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        image_tag_t *v = mz_malloc(sizeof(*v));

        char *org_x = mz_xml_node_attribute(cur, "originX");
        char *org_y = mz_xml_node_attribute(cur, "originY");
        
        v->filepath = mz_xml_node_value(cur);
        v->origin_x = mz_atoi(org_x, 0);
        v->origin_y = mz_atoi(org_y, 0);

        mz_free(org_x);
        mz_free(org_y);

        items[count++] = v;
        if (count == nitem) {
            nitem += nitem;
            items = mz_realloc(items, sizeof(items[0]) * nitem);
        }

        cur = mz_xml_find_next_node(cur, node_name);
    }

    items = mz_realloc(items, sizeof(items[0]) * count);

    frame->images = items;
    frame->nimage = count;
}

static void read_sequence(mz_xml_node_t node, animation_tag_t *sequence)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "frame";

    int nitem = 4;
    int count = 0;
    frame_tag_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        frame_tag_t *v = mz_malloc(sizeof(*v));
        char *duration = mz_xml_node_attribute(cur, "duration");

        v->duration = mz_atoi(duration, 100);
        mz_free(duration);

        read_image(cur, v);

        items[count++] = v;
        if (count == nitem) {
            nitem += nitem;
            items = mz_realloc(items, sizeof(items[0]) * nitem);
        }

        cur = mz_xml_find_next_node(cur, node_name);
    }   
    
    items = mz_realloc(items, sizeof(items[0]) * count);

    sequence->frames = items;
    sequence->nframe = count;
}

static void split_name_and_stat(const char *name_prop, char **name, char **stat)
{
    if (mz_strchr(name_prop, '.') == NULL) {
        *name = mz_strdup(name_prop);
        *stat = NULL;
    }
    else {
        char *tok = mz_strdup(name_prop);

        *name = mz_strdup(mz_strtrim(mz_strtok(tok, "."), " "));
        *stat = mz_strdup(mz_strtrim(mz_strtok(NULL, "."), " "));

        mz_free(tok);
    }
}

static void read_animation_set(mz_xml_node_t node, animation_set_tag_t *animation)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "animation";

    int nitem = 4;
    int count = 0;
    animation_tag_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        animation_tag_t *v = mz_malloc(sizeof(*v));
        char *name_prop = mz_xml_node_attribute(cur, "name");

        split_name_and_stat(name_prop, &v->name, &v->stat);
        mz_free(name_prop);

        read_sequence(cur, v);

        items[count++] = v;
        if (count == nitem) {
            nitem += nitem;
            items = mz_realloc(items, sizeof(items[0]) * nitem);
        }

        cur = mz_xml_find_next_node(cur, node_name);
    }   

    items = mz_realloc(items, sizeof(items[0]) * count);

    animation->animations = items;
    animation->nanimation = count;
}

static animation_set_tag_t* parse_xml(const char *xml_file)
{
    animation_set_tag_t* self = mz_malloc(sizeof(*self));
    
    mz_xml_t *xml = mz_xml_load(xml_file);
    mz_xml_node_t cur = mz_xml_get_root_node(xml);

    if (!cur) {
        logI("Can't read the root element: %s\n", xml_file);
        goto error;
    }

    if (!mz_xml_is_tag(cur, "package")) {
        logI("Root node tag should be -> %s", "package");
        goto error;
    }

    cur = mz_xml_children_node(cur);
    cur = mz_xml_find_next_node(cur, "animationSet");

    if (!cur) {
        logI("Can not find tag -> %s", "animationSet");
        goto error;
    }

    read_animation_set(cur, self);

    return self;

error:
    if (xml)
        mz_xml_delete(xml);

    return NULL;
}

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

typedef struct ani_info_t
{
    const char  **images;
    int         image_count;
} ani_info_t;

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

        p->x = image_tag->origin_x;
        p->y = image_tag->origin_y;
        p->image_id = get_array_id(info->images, info->image_count, image_tag->filepath);
        

        if (item_id == nitem) {
            nitem += nitem;
            frame->patterns = mz_realloc(frame->patterns, sizeof(frame->patterns[0]) * nitem);
        }
    }

    frame->duration = frame_tag->duration;
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

    sequence->stat = ani_tag->stat;
};

static void generate_one_ani_file(animation_set_tag_t *set, const char *anim_name)
{
    ani_info_t ani_info = {0};
    int nitem = 4;
    int item_id = 0;
    int i;

    mz_animation_t *animation = mz_malloc(sizeof(*animation));
    animation->sequences = mz_malloc(sizeof(animation->sequences[0]) * nitem);

    get_unique_image_names_in_one_animation(set, anim_name, &ani_info.images, &ani_info.image_count);

    logI("image count -> %d", ani_info.image_count);
    for (i = 0; i < ani_info.image_count; i++)
        logI("image file -> %s", ani_info.images[i]);

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
}

MZ_API int mz_animation_generate_ani_file(const char *xml_file, const char *dst_folder)
{
    animation_set_tag_t *set = parse_xml(xml_file);
    const char **animation_names;
    int animation_count;
    int i;

    get_sequence_name_list(set, &animation_names, &animation_count);

    for (i = 0; i < animation_count; i++) 
        generate_one_ani_file(set, animation_names[i]);

    return 0;
}

MZ_API animation_set_tag_t* mz_animation_load(const char *ani_file)
{
    return NULL;
}
