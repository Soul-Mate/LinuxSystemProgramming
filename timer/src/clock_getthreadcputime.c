

#include "main.h"
#include <time.h>
#include <pthread.h>

void *run(void *arg)
{
  sleep(3);
  int i;
  for(i = 0; i < 3000000; i++) {
  }
  return NULL;
}

int main(int argc, char const *argv[])
{
  clockid_t thread_clockid;
  struct timespec tp;
  time_t start = time(NULL);
  printf("start time: %s\n", ctime(&start));
  pthread_t thread;
  if (pthread_create(&thread,NULL,run,NULL) != 0)
    err_quit("pthread_create error");
  if (pthread_join(thread,NULL) != 0)
    err_quit("pthread_join error");
  pthread_getcpuclockid(thread,&thread_clockid);
  clock_gettime(thread_clockid,&tp);
  printf("end time: %s\n", ctime(&tp.tv_sec));
  return 0;
}
