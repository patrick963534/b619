#include <stdio.h>
#include <mz/director.h>
#include <mz/node.h>
#include <mz/actor.h>
#include <mz/scene.h>
#include <mz/libs.h>
#include <mz/event.h>
#include <mz/animation.h>

typedef struct test_scene_t
{
    extends_scene();

    mz_actor_t *actor;
    mz_actor_t *actor2;
} test_scene_t;

static void step(mz_node_t *self, int ellapse)
{
    mz_unused(ellapse);
}

static int on(mz_node_t *self_, mz_event_t *e)
{
    if (e->type == mz.events.KeyDown)
    {
        mz_downcast(test_scene_t);

        if (e->keyboard.keycode == mz.keys.Left)
            self->actor->x -= 10;
        else if (e->keyboard.keycode == mz.keys.Right)
            self->actor->x += 10;
    }

    return 0;
}

mz_scene_t* mz_main()
{
    mz_animation_generate_ani_file("res/animation/basic.xml", "res/animation/");

    test_scene_t *scene = (test_scene_t*)mz_scene_new(sizeof(*scene), NULL);

    scene->actor = mz_actor_new("res/animation/fruite.ani", sizeof(*scene->actor), (mz_node_t*)scene);
    scene->actor2 = mz_actor_new("res/animation/fruite.ani", sizeof(*scene->actor2), (mz_node_t*)scene);

    scene->actor->x = 75;
    scene->actor->y = 75;

    scene->actor2->x = 200;
    scene->actor2->y = 200;

    scene->on = on;
    scene->step = step;

	return (mz_scene_t*)scene;
}
