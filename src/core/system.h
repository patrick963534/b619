#ifndef __MZ_SYSTEM_H__
#define __MZ_SYSTEM_H__

#include <mz/event.h>

typedef struct mz_system_t
{
	void (*wait_event)(mz_event_t* event);
} mz_system_t;

void mz_system_init();
void mz_system_quit();
mz_system_t *mz_system_instance();

#endif
