/*
函数alarm实现思路：
使用setitimer设置定时器 到期发送SIGALRM
*/

#include "main.h"
#include <time.h>
#include <sys/time.h>

unsigned int user_alarm(unsigned int seconds)
{
  struct itimerval new_value,old_value;
  new_value.it_value.tv_sec = seconds;
  new_value.it_value.tv_usec = 0;
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL,&new_value,&old_value);
  return old_value.it_value.tv_sec;
}

void sig_handler(int signo)
{
  if (signo == SIGALRM) {
    printf("caught SIGALRM signo\n");
  }
}


int main(int argc, char const *argv[])
{
  sigset_t sig_mask;
  sigemptyset(&sig_mask);
  struct sigaction sa;
  sa.sa_handler = sig_handler;
  sa.sa_mask = sig_mask;
  sa.sa_flags |= SA_RESTART;
  sigaction(SIGALRM,&sa,NULL);
  printf("%u\n", user_alarm(3));
  printf("%u\n", user_alarm(5));
  for(;;);
  return 0;
  return 0;
}
