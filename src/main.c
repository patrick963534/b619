#include <stdio.h>
#include <mz/director.h>
#include <mz/node.h>
#include <mz/actor.h>
#include <mz/scene.h>
#include <mz/libs.h>
#include <mz/event.h>
#include <mz/event_node.h>

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

int main(int argc, char** argv)
{
    mz_scene_t *scene = mz_scene_new(sizeof(*scene), NULL);

    mz_actor_t *actor1 = mz_actor_new("/home/patrick/image/1.png", sizeof(*actor1), (mz_node_t*)scene);
    mz_actor_t *actor2 = mz_actor_new("/home/patrick/image/2.png", sizeof(*actor2), (mz_node_t*)scene);
    mz_actor_t *actor3 = mz_actor_new("/home/patrick/image/31.png", sizeof(*actor3), (mz_node_t*)scene);

    mz_event_node_t *ev1 = mz_event_node_new(sizeof(*ev1), (mz_node_t*)actor1);
    mz_event_node_t *ev2 = mz_event_node_new(sizeof(*ev2), (mz_node_t*)actor2);
    mz_event_node_t *ev3 = mz_event_node_new(sizeof(*ev3), (mz_node_t*)actor3);
    ev1->event = on;
    ev1->step = step1;
    ev2->step = step2;
    ev3->step = step3;

    actor1->x = 250;

    mz_director_push(scene);
	mz_director_loop();

    mz_unused(argc);
    mz_unused(argv);

	return 0;
}
