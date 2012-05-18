#include <mz/animation.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/simple_xml.h>

#define BUFFERSIZE 1024 * 1024 * 3
#define READALINE 1024 * 1024 * 1

static void read_image(mz_xml_node_t node, mz_frame_t *frame)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "image";

    int nitem = 4;
    int count = 0;
    mz_image_tag_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        mz_image_tag_t *v = mz_malloc(sizeof(*v));

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

static void read_sequence(mz_xml_node_t node, mz_sequence_t *sequence)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "frame";

    int nitem = 4;
    int count = 0;
    mz_frame_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        mz_frame_t *v = mz_malloc(sizeof(*v));
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

static void read_animation_set(mz_xml_node_t node, mz_animation_t *animation)
{
    mz_xml_node_t cur = mz_xml_children_node(node);
    const char *node_name = "animation";

    int nitem = 4;
    int count = 0;
    mz_sequence_t **items = mz_malloc(sizeof(items[0]) * nitem);

    if (!mz_xml_is_tag(cur, node_name))
        cur = mz_xml_find_next_node(cur, node_name); 

    while (cur != NULL) {
        mz_sequence_t *v = mz_malloc(sizeof(*v));
        v->name = mz_xml_node_attribute(cur, "name");

        read_sequence(cur, v);

        items[count++] = v;
        if (count == nitem) {
            nitem += nitem;
            items = mz_realloc(items, sizeof(items[0]) * nitem);
        }

        cur = mz_xml_find_next_node(cur, node_name);
    }   

    items = mz_realloc(items, sizeof(items[0]) * count);

    animation->sequences = items;
    animation->nsequence = count;
}

MZ_API mz_animation_t* mz_animation_load(const char *file)
{
    mz_animation_t* self = mz_malloc(sizeof(*self));
    
    mz_xml_t *xml = mz_xml_load(file);
    mz_xml_node_t cur = mz_xml_get_root_node(xml);

    if (!cur) {
        logI("Can't read the root element: %s\n", file);
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

error:
    if (xml)
        mz_xml_delete(xml);

    return self;
}
