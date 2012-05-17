#include <mz/animation.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <mz/simple_xml.h>

#define BUFFERSIZE 1024 * 1024 * 3
#define READALINE 1024 * 1024 * 1

static void read_sequence(mz_xml_node_t node)
{

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

    do
    {
        logI("current node name -> %s", mz_xml_node_name(cur));

        if (mz_xml_is_tag(cur, "package")) 
            read_sequence(cur);

        cur = mz_xml_next_node(cur);
    } while (cur != NULL);

error:
    if (xml)
        mz_xml_delete(xml);

    return self;
}
