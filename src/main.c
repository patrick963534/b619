#include <stdio.h>
#include <mz/director.h>
#include <mz/node.h>
#include <mz/actor.h>
#include <mz/scene.h>

int main(int argc, char** argv)
{
	(void)(argc);
	(void)(argv);

    mz_scene_t *scene = mz_scene_new(sizeof(*scene), NULL);

    mz_actor_t *actor1 = mz_actor_new("/home/patrick/image/1.png", sizeof(*actor1), (mz_node_t*)scene);
    mz_actor_t *actor2 = mz_actor_new("/home/patrick/image/1.png", sizeof(*actor2), (mz_node_t*)scene);
    mz_actor_t *actor3 = mz_actor_new("/home/patrick/image/1.png", sizeof(*actor3), (mz_node_t*)scene);
    actor1->name = "1st";
    actor2->name = "2st";
    actor3->name = "3st";

    mz_director_push(scene);
	mz_director_loop();

	return 0;
}
