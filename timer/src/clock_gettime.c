/*
#define _POSIX_C_SOURCE 199309
#include <time.h>

int clock

*/

#define _POSIX_C_SOURCE 199309

#include "main.h"
#include <time.h>


void print_time(const char *format,const struct tm *timeptr)
{
  char *out_put;
  out_put = malloc(1024);
  strftime(out_put,1024,format,timeptr);
  printf("%s\n", out_put);
  free(out_put);
}

int main(int argc, const char *argv[])
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME,&ts);
  struct tm *timeptr = gmtime(&ts.tv_sec);
  char * format = "%Y年%m月%d日 %H时%M分%S秒";
  timeptr->tm_hour += 8;
  print_time(format,timeptr);
}
