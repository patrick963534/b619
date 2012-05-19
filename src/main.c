#include <stdio.h>
#include <mz/director.h>
#include <mz/node.h>
#include <mz/actor.h>
#include <mz/scene.h>
#include <mz/libs.h>
#include <mz/event.h>
#include <mz/animation.h>

static void step1(mz_node_t *self, int ellapse)
{
    mz_unused(ellapse);

    if (self->x > 600)
        self->x = 0;

    self->x += 5;
}

static void step2(mz_node_t *self, int ellapse)
{
    mz_unused(ellapse);

    if (self->x > 600)
        self->x = 0;

    self->x += 15;
}

static void step3(mz_node_t *self, int ellapse)
{
    mz_unused(ellapse);

    if (self->x > 600)
        self->x = 0;

    self->x += 10;
}

static int on(mz_node_t *self, mz_event_t *e)
{
    if (e->type == mz.events.KeyDown)
    {
        if (e->keyboard.keycode == mz.keys.Left)
            self->x -= 10;
        else if (e->keyboard.keycode == mz.keys.Right)
            self->x += 10;
    }

    return 0;
}

mz_scene_t* mz_main()
{

    //mz_animation_t *animation = mz_animation_load("/home/patrick/pluto/project/libmz/res/animation/basic.xml");

    //mz_animation_generate_ani_file("/home/patrick/pluto/project/libmz/res/animation/basic.xml", NULL);

    mz_scene_t *scene = mz_scene_new(sizeof(*scene), NULL);

    mz_actor_t *actor = mz_actor_new("res/animation/4.png", sizeof(*actor), (mz_node_t*)scene);
    mz_actor_t *actor1 = mz_actor_new("res/animation/1.png", sizeof(*actor1), (mz_node_t*)scene);
    mz_actor_t *actor2 = mz_actor_new("res/animation/2.png", sizeof(*actor2), (mz_node_t*)scene);
    mz_actor_t *actor3 = mz_actor_new("res/animation/3.png", sizeof(*actor3), (mz_node_t*)scene);

    actor1->on = on;
    actor1->step = step1;
    actor2->step = step2;
    actor3->step = step3;

    actor1->y = 50;
    actor2->y = 100;
    actor3->y = 150;

    actor1->x = 250;

	return scene;
}
