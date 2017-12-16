#include "main.h"
#include <time.h>
#include <sys/time.h>

void
print_tm(struct tm *t)
{
	printf("年: %d\n", 1900 + t->tm_year);
	printf("月: %d\n", t->tm_mon + 1);
	printf("日: %d\n", t->tm_mday);
	printf("时: %d\n", t->tm_hour + 8);
	printf("分: %d\n", t->tm_min);
	printf("秒: %d\n", t->tm_sec + 1);
}

int
main(int argc, char const *argv[])
{
	struct tm      *t1,*t2;
	struct timeval tv;
	if (gettimeofday(&tv,NULL) < 0)
		err_msg("gettimeofday error");
	t1 = (struct tm *)malloc(sizeof(struct tm));
	t1 = gmtime(&tv.tv_sec);
	printf("---------- gmtime ----------\n" );
	print_tm(t1);
	printf("---------- localtime ----------\n" );
	t2 = (struct tm *)malloc(sizeof(struct tm));
	t2 = localtime(&tv.tv_sec);
	print_tm(t2);
	printf("---------- mktime t2 ----------\n" );
	printf("%lu\n", mktime(t2));
	return 0;
}