#include <unistd.h>
#include "../util.c.h"

void mz_sleep(int ms)
{
	usleep(ms * 1000);
}
