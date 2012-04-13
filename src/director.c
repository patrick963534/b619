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
static mz_scene_t *scene_;

static void director_render()
{
    mz_actor_t *actor;

    mz_graphics_clear();

    list_for_each_entry(actor, &scene_->element_, mz_actor_t, element_) 
    {
        mz_graphics_draw_texture(actor->texture);
    }

    mz_graphics_flush();
}

static void director_update()
{
    logI("Update");
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
		
	}
}

MZ_API void mz_director_push(mz_scene_t *scene)
{
    scene_ = scene;
}

