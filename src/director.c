#include <mz/director.h>
#include <mz/scene.h>
#include <mz/defs.h>
#include <mz/libs.h>
#include <mz/actor.h>
#include <mz/object.h>
#include <mz/image.h>
#include "core/system.h"
#include "core/graphics.h"
#include "event.h"

static mz_director_t* g_director;

static void director_render()
{
    mz_graphics_clear();

    mz_node_draw((mz_node_t*)g_director->cur_scene);

    mz_graphics_flush();
}

static void director_update()
{
    mz_node_step((mz_node_t*)g_director->cur_scene, 0);
}

static void director_event(mz_event_t *e)
{
    // process filters first.

    mz_node_on((mz_node_t*)g_director->cur_scene, e);
}

mz_director_t* mz_director_new(size_t size, mz_scene_t* scene)
{
    mz_director_t *v = mz_malloc(size);

    v->cur_scene_id = 0;
    v->scene_stack[0] = scene;
    v->cur_scene = scene;

    return v;
}

void mz_director_pop()
{
    if (g_director->cur_scene_id < 1)
        return;

    mz_object_delete((mz_object_t*)g_director->cur_scene);

    g_director->cur_scene_id--;
    g_director->cur_scene = g_director->scene_stack[g_director->cur_scene_id];
}

void mz_director_push(mz_scene_t *scene)
{
    g_director->cur_scene = scene;
    g_director->cur_scene_id++;
    g_director->scene_stack[g_director->cur_scene_id] = scene;
}

void mz_director_clean(mz_scene_t *scene)
{
    do
    {
        mz_director_pop();
    } while (g_director->cur_scene_id != 0);

    mz_object_delete((mz_object_t*)g_director->cur_scene);
}

static void mz_exit()
{
    mz_object_delete((mz_object_t*)g_director);
}

int main(int argc, char** argv)
{ 
    mz_system_init();

    g_director = mz_director_new(sizeof(*g_director), mz_main());
	
	while (g_director->cur_scene) {
		mz_event_t event = {0};

		director_render();

		mz_system_instance()->wait_event(&event);

        if (event.type == mz.events.UpdateFrame)
			director_update();
        else if (event.type == mz.events.KeyUp && event.keyboard.keycode == mz.keys.Escape)
			break;
        else
            director_event(&event);
	}

    mz_exit();

    return 0;
}
