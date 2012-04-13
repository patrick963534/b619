#include <mz/node.h>
#include <mz/container.h>

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent)
{
    mz_node_t *v = (mz_node_t*)mz_container_new(size);
    if (parent)
        mz_node_add_child(parent, v);

    return v;
}

MZ_API void mz_node_add_child(mz_node_t *self, mz_node_t *child)
{
    mz_container_add((mz_container_t*)self, (mz_object_t*)child);
    child->parent = self;
}
