#include "main.h"
#include <sys/time.h>
#include <time.h>

int 
main(int argc, char const *argv[])
{
	struct timeval tv;
	time_t timep;
	timep = time(NULL);
	printf("time: %lu\n", timep);
	if (gettimeofday(&tv,NULL) < 0)
		err_msg("gettimeofday error");
	printf("gettimeofday: %lu\n",tv.tv_sec);
	return 0;
}