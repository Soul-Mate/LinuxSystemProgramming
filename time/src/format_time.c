#include "main.h"
#include <sys/time.h>
#include <time.h>

void print_ctime(const time_t *timep)
{
	printf("---------- ctime ----------\n" );
	printf("%s\n", ctime(timep));
}

void print_asctime(const struct tm *timeptr)
{
	printf("---------- asctime ----------\n" );
	printf("%s\n", asctime(timeptr));
}

void print_strftime(const char *format,const struct tm *timeptr)
{
	printf("---------- strftime ----------\n" );
	char *out_str;
	out_str = malloc(1000);
	if (strftime(out_str,1000,format,timeptr) > 0) {
		printf("%s\n", out_str);
	}
}

int
main(int argc, char const *argv[])
{
	time_t timep = time(NULL);
	struct tm *timeptr = gmtime(&timep); 
	const char * format = "%Y年%m月%d日 %H时%M分%S秒\n";
	timeptr->tm_hour += 8;
	timep = mktime(timeptr);
	print_ctime(&timep);
	print_asctime(timeptr);
	print_strftime(format,timeptr);
	return 0;
}