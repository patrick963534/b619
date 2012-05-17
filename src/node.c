#include <mz/node.h>
#include <mz/object.h>
#include <mz/container.h>
#include <mz/libs.h>
#include <mz/event.h>

int mz_node_on(mz_node_t *self, mz_event_t *e)
{
    mz_node_t *sub_node;

    if (self->on && self->on(self, e))
        return 1;

    list_for_each_entry(sub_node, &self->children, mz_node_t, element)
    {
        if (mz_node_on(sub_node, e))
            return 1;
    }

    return 0;
}

void mz_node_step(mz_node_t *self, int ellapse)
{
    mz_node_t *sub_node;

    if (self->step)
        self->step(self, ellapse);

    list_for_each_entry(sub_node, &self->children, mz_node_t, element)
    {
        mz_node_step(sub_node, ellapse);
    }
}

void mz_node_draw(mz_node_t *self)
{
    mz_node_t *node;

    list_for_each_entry(node, &self->children, mz_node_t, element) 
    {
        if (node->draw)
            node->draw(node);
    }
}

MZ_API void mz_node_destruct(mz_object_t* self_)
{
    mz_object_destruct(self_);
}

MZ_API mz_node_t* mz_node_new(size_t size, mz_node_t *parent)
{
    mz_node_t *v = (mz_node_t*)mz_container_new(size);

    if (parent)
        mz_node_add_child(parent, v);

    v->destruct = mz_node_destruct;

    return v;
}

MZ_API void mz_node_add_child(mz_node_t *self, mz_node_t *child)
{
    mz_container_add((mz_container_t*)self, (mz_object_t*)child);
    child->parent = self;
}
