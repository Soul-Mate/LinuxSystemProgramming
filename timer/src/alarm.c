/*
更简单的定时器接口
#include <sys/time.h>
usigned int alarm(unsigned int seconds);
        成功返回定时器的秒数,失败返回0
alarm到期时会产生SIGALRM信号
*/

#include "main.h"
#include <time.h>
#include <sys/time.h>


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
  alarm(1);
  for(;;);
  return 0;
}
