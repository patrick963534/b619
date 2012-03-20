#include <mz/director.h>
#include <mz/defs.h>
#include "core/system.h"
#include "event.h"

static int is_system_init;

static void director_render()
{

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

		printf("loop\n");

		if (event.type == mz.types.key_up && event.keyboard.keycode == mz.keys.Escape)
			return;
		else if (event.type == mz.types.update_frame)
			director_update();

		
	}
}

MZ_API void mz_director_push(mz_scene_t *scene)
{
}

