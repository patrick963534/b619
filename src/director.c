#include <mz/director.h>
#include <mz/scene.h>
#include <mz/defs.h>
#include "core/system.h"

#include "event.h"

static int is_system_init;
static mz_scene_t *scene_;

static void director_render()
{
    printf("render");
}

static void director_update()
{

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

        if (event.type == mz.types.UpdateFrame)
			director_update();
        else if (event.type == mz.types.KeyUp && event.keyboard.keycode == mz.keys.Escape)
			break;
		
	}
}

MZ_API void mz_director_push(mz_scene_t *scene)
{
    scene_ = scene;
}

