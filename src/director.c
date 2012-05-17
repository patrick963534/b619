#include <mz/director.h>
#include <mz/scene.h>
#include <mz/defs.h>
#include <mz/libs.h>
#include <mz/actor.h>
#include "core/system.h"
#include "core/texture.h"
#include "core/graphics.h"
#include "event.h"

static int is_system_init;
static mz_scene_t *g_scene;

static void director_render()
{
    mz_graphics_clear();

    mz_node_draw((mz_node_t*)g_scene);

    mz_graphics_flush();
}

static void director_update()
{
    mz_node_step((mz_node_t*)g_scene, 0);
}

static void director_event(mz_event_t *e)
{
    // process filters first.

    mz_node_on((mz_node_t*)g_scene, e);
}

MZ_API void mz_director_loop()
{ 
	if (!is_system_init) {
		mz_system_init();
		is_system_init = 1;
	}
	
	while (1) {
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
}

MZ_API void mz_director_push(mz_scene_t *scene)
{
    g_scene = scene;
}

