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
    frame_tag_t *cur_frame;
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

static int is_exist(const char **names, int count, const char *target)
{
    int i;

    for (i = 0; i < count; i++)
        if (mz_strcmp(names[i], target) == 0)
            return 1;

    return 0;
}

static void get_sequence_name_list(animation_set_tag_t *set, const char ***ret_names, int *ret_count)
{
    const char **names = mz_malloc(sizeof(names[0]) * set->nanimation);
    int count = 0;
    int i;
    
    for (i = 0; i < set->nanimation;  i++) {
        if (is_exist(names, count, set->animations[i]->name))
            continue;

        names[count++] = mz_strdup(set->animations[i]->name);
    }

    names = mz_realloc(names, sizeof(names[0]) * count);

    *ret_names = names;
    *ret_count = count;
}

static void generate_one_ani_file(animation_set_tag_t *set, const char *anim_name)
{
    int nimage = 4;
    const char **images = mz_malloc(sizeof(images[0]) * nimage);
    int image_count = 0;

    int i, j, k, h;

    for (i = 0; i < set->nanimation; i++) {
        animation_tag_t *anim = set->animations[i];

        if (mz_strcmp(anim->name, anim_name) != 0)
            continue;

        logI("src->%s --- dst->%s", anim->name, anim_name);

        for (j = 0; j < anim->nframe; j++) {
            frame_tag_t *frame = anim->frames[j];

            for (k = 0; k < frame->nimage; k++) {
                image_tag_t *img = frame->images[k];
                
                if (is_exist(images, image_count, img->filepath))
                    continue;

                images[image_count++] = mz_strdup(img->filepath);

                if (image_count == nimage) {
                    nimage += nimage;
                    images = mz_realloc(images, sizeof(images[0]) * nimage);
                }
            }
        }
    }

    for (i = 0; i < image_count; i++)
        logI("image file -> %s", images[i]);

    logI("image count -> %d", image_count);
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
