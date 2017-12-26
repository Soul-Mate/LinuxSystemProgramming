/*
#include <sys/time.h>
int getitimer(int witch,struct itimerval *curr_value);
        成功返回0,失败返回-1
调用该函数通过itimeval获取定时器状态,和setitimer函数的old_value是一样的,
区别在于无需改变定时器的设置
*/

#include "main.h"
#include <time.h>
#include <sys/time.h>

void sig_handler(int signo)
{

  if (signo == SIGALRM) {
    struct itimerval old_value;
    getitimer(ITIMER_REAL,&old_value);
    printf("caught SIGALRM\n");

    printf("interval sec: %ld\n", old_value.it_interval.tv_sec);
    printf("interval usec: %ld\n", old_value.it_interval.tv_usec);
    printf("itvalue sec: %ld\n", old_value.it_value.tv_sec);
    printf("itvalue usec: %ld\n", old_value.it_value.tv_sec);
  }
}

int main(int argc, char const *argv[])
{
  struct sigaction sa;
  struct itimerval new_value;
  sigset_t sig_mask;
  sigemptyset(&sig_mask);
  sa.sa_handler = sig_handler;
  sa.sa_mask = sig_mask;
  sa.sa_flags |= SA_RESTART;

  new_value.it_interval.tv_sec = 1;
  new_value.it_interval.tv_usec = 0;
  new_value.it_value.tv_sec = 1;
  new_value.it_value.tv_usec = 0;
  sigaction(SIGALRM,&sa,NULL);
  setitimer(ITIMER_REAL,&new_value,NULL);
  for(;;);
  return 0;
}
