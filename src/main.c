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
    mz_actor_t *actor = mz_actor_new(NULL, sizeof(*actor), (mz_node_t*)scene);

	mz_director_loop();

	return 0;
}
