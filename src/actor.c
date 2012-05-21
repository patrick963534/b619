#include <mz/actor.h>
#include <mz/node.h>
#include <mz/image.h>
#include <mz/defs.h>
#include <mz/animation.h>
#include "core/graphics.h"

MZ_API void mz_actor_draw(mz_node_t *self_)
{
    mz_downcast(mz_actor_t);
    int i;

    int img_id;
    mz_image_t *img;
    mz_frame_t *frame;

    frame = self->animation->cur_sequence->cur_frame;
    if (!frame)
        return;

    for (i = 0; i < frame->npattern; i++) {
        int x = self->x - frame->patterns[i]->origin_x;
        int y = self->y - frame->patterns[i]->origin_y;

        img_id = frame->patterns[i]->image_id;
        img = self->animation->images[img_id];

        mz_graphics_draw_texture(img, x, y);
    }
}

MZ_API void mz_actor_destruct(mz_object_t* self_)
{
    mz_node_destruct((mz_node_t*)self_);
}

MZ_API void mz_actor_step(mz_node_t *self_, int ellapse)
{
    mz_downcast(mz_actor_t);
    mz_animation_t *ani = self->animation;
    mz_sequence_t *seq = ani->sequences[ani->cur_sequence_id];

    if (++seq->cur_frame_id >= seq->nframe)
        seq->cur_frame_id = 0;

    seq->cur_frame = seq->frames[seq->cur_frame_id];
}

MZ_API mz_actor_t* mz_actor_new(const char *ani_file, size_t size, mz_node_t *parent)
{
    mz_actor_t *v = (mz_actor_t*)mz_node_new(size, parent);
    v->animation = mz_animation_load(ani_file);

    v->draw = mz_actor_draw;
    v->step = mz_actor_step;
    v->destruct = mz_actor_destruct;

    return v;
}
