#include <stdio.h>
#include <mz/director.h>

int main(int argc, char** argv)
{
	(void)(argc);
	(void)(argv);

	mz_director_loop();

	return 0;
}
