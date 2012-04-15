#include <mz/event_node.h>
#include <mz/node.h>
#include <mz/event.h>

typedef struct event_node_vtable_t
{
    extends_event_node_vtable();
} event_node_vtable_t;

static void draw(mz_node_t *self_)
{
    mz_downcast(mz_event_node_t);
    if (self->draw)
        self->draw(self_->parent);
}

static int event_on(mz_node_t *self_, mz_event_t *e)
{
    mz_downcast(mz_event_node_t);

    if (self->event)
        return self->event(self->parent, e);

    return 0;
}

static void step(mz_node_t *self_, int ellapse)
{
    mz_downcast(mz_event_node_t);
    if (self->step)
        self->step(self_->parent, ellapse);
}

MZ_API mz_vtable_t* mz_event_node_get_vtable()
{
    static event_node_vtable_t vtable;

    if (vtable.size == 0)
    {
        mz_vtable_t *base_vtable = mz_node_get_vtable();
        mz_memcpy(&vtable, base_vtable, base_vtable->size);

        vtable.draw = draw;
        vtable.event = event_on;
        vtable.step = step;
        vtable.size = sizeof(vtable);
    }

    return (mz_vtable_t*)&vtable;
}

MZ_API mz_event_node_t* mz_event_node_new(size_t size, mz_node_t *parent)
{
    mz_event_node_t *v = (mz_event_node_t*)mz_node_new(size, parent);
    v->vtable = mz_event_node_get_vtable();

    return v;
}
