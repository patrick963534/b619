#include <mz/simple_xml.h>
#include <mz/libs.h>
#include <mz/defs.h>
#include <libxml/parser.h>

MZ_API mz_xml_t* mz_xml_load(const char *file)
{
    mz_xml_t *self = mz_malloc(sizeof(*self));
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;

    ctxt = xmlNewParserCtxt();

    if (doc == NULL) {
        logI("Can't parse the content: %s\n", file);
        goto error;
    }

    doc = xmlCtxtReadFile(ctxt, file, "UTF8", XML_PARSE_DTDATTR|XML_PARSE_NOERROR);

    self->ctxt = (void*)ctxt;
    self->doc = (void*)doc;

    return self;

error:
    if (self)
        mz_free(self);

    return NULL;
}

MZ_API mz_xml_node_t mz_xml_get_root_node(mz_xml_t *xml)
{
    mz_assert(xml);

    return (mz_xml_node_t)xmlDocGetRootElement((xmlDocPtr)xml->doc);
}

MZ_API mz_xml_node_t mz_xml_next_node(mz_xml_node_t node)
{
    mz_assert(node);

    return (mz_xml_node_t)(((xmlNodePtr)node)->next);
}

MZ_API mz_xml_node_t mz_xml_children_node(mz_xml_node_t node)
{
    mz_assert(node);

    return (mz_xml_node_t)(((xmlNodePtr)node)->xmlChildrenNode);
}

MZ_API int mz_xml_is_tag(mz_xml_node_t node, const char *tag)
{
    mz_assert(node);

    return xmlStrcmp(((xmlNodePtr)node)->name, (const xmlChar*)tag) == 0;
}

MZ_API void mz_xml_delete(mz_xml_t *xml)
{
    if (xml == NULL)
        return;

    xmlFreeDoc((xmlDocPtr)xml->doc);
    xmlFreeParserCtxt((xmlParserCtxtPtr)xml->ctxt);
}

MZ_API char* mz_xml_node_name(mz_xml_node_t node)
{
    const xmlChar *tag = (const char*)((xmlNodePtr)node)->name;
    char *result;

    if (!tag)
        return NULL;

    result = mz_strdup(tag);

    return result;
}

MZ_API char* mz_xml_node_attribute(mz_xml_node_t node, const char *attribute_name)
{
    xmlChar *attr = xmlGetProp((xmlNodePtr)node, attribute_name);
    char *result;

    if (!attr)
        return NULL;
    
    result = mz_strdup((const char*)attr);

    xmlFree(attr);

    return result;
}

MZ_API char* mz_xml_node_value(mz_xml_node_t node)
{
    xmlChar *attr = xmlNodeGetContent((xmlNodePtr)node);
    char *result;

    if (!attr)
        return NULL;
    
    result = mz_strdup((const char*)attr);

    xmlFree(attr);

    return result;

}
MZ_API mz_xml_node_t mz_xml_find_next_node(mz_xml_node_t node, const char *tag)
{
    assert(node);

    node = mz_xml_next_node(node);

    while (node)
    {
        if (mz_xml_is_tag(node, tag))
            return node;

        node = mz_xml_next_node(node);
    }

    return NULL;
}
