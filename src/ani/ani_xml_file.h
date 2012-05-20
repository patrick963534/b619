#ifndef __MZ_ANI_XML_FILE_H__
#define __MZ_ANI_XML_FILE_H__

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

animation_set_tag_t* parse_xml(const char *xml_file);

#endif
