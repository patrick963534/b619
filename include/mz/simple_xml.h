#ifndef __MZ_SIMPLE_XML_H__
#define __MZ_SIMPLE_XML_H__

#include <mz/defs.h>

typedef struct mz_xml_t
{
    void *ctxt;
    void *doc;
} mz_xml_t;

typedef void* mz_xml_node_t;

MZ_API mz_xml_t*        mz_xml_load(const char *file);
MZ_API void             mz_xml_delete(mz_xml_t *xml);
MZ_API mz_xml_node_t    mz_xml_get_root_node(mz_xml_t *xml);
MZ_API mz_xml_node_t    mz_xml_next_node(mz_xml_node_t node);
MZ_API mz_xml_node_t    mz_xml_children_node(mz_xml_node_t node);
MZ_API int              mz_xml_is_tag(mz_xml_node_t node, const char *tag);
MZ_API char*            mz_xml_node_name(mz_xml_node_t node);
MZ_API char*            mz_xml_node_attribute(mz_xml_node_t node, const char* attribute_name);
MZ_API char*            mz_xml_node_value(mz_xml_node_t node);
MZ_API mz_xml_node_t mz_xml_find_next_node(mz_xml_node_t node, const char *tag);

#endif
