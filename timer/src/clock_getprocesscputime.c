#define _XOPEN_SOURCE 600

#include "main.h"
#include <time.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
  struct timespec tp;
  clockid_t clockid;
  pid_t pid;
  pid = fork();
  time_t start = time(NULL);
  if (pid < 0)
    err_quit("fork error");
  if (pid == 0) {
    sleep(1);
    int i;
    for (i = 0; i < 3000000; i++) {
      printf("%d\n", i);
    }
  } else if(pid > 0) {
    wait(NULL);
    clock_getcpuclockid(pid,&clockid);
    clock_gettime(clockid,&tp);
    printf("The process(%d) consume cpu time: %ld second\n",
    pid,tp.tv_sec - start);
  }

  return 0;
}
