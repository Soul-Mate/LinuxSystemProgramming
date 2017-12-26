/*
间隔定时器
#include <sys/time.h>
int setitimer(int witch, const struct itimerval *new_value,
    struct itimerval *old_value);
        成功返回0,失败返回-1

struct itimeval {
    struct timeval it_interval;  第一次之后每隔多长时间
    struct timeval it_value;  第一次调用要多长时间
}

struct timeval {
    time_t tv_sec;   1970年到现在的秒数
    suseconds_t tv_usec;  微妙
}

old_value 是上一个定时器的传出值

定时器是根据it_value递减为0之后发送信号,之后再根据it_interval重新定时,new_value 中的
it_value的值不能为 0 为0定时器不启动.

witch可选类型
ITIMER_REAL
  创建以真实时间倒计时的定时器。到期时会产生SIGALRM信号
ITIMER_VIRTUAL
  创建虚拟进程时间(用户模式下CPU时间)倒计时的定时器。到期时会产生SIGVTALRM信号。
ITIMER_PROF
  创建一个profiling进程，以进程时间(用户态和内核态CPU时间总和)倒计时,
到期时产生SIGPROF信号
*/

#include "main.h"
#include <time.h>
#include <sys/time.h>

void sig_handler(int signo)
{
    printf("hello world!\n");
}


int main(int argc, char const *argv[])
{
  sigset_t sig_mask;
  struct sigaction sa;
  struct itimerval new_time;
  if (sigemptyset(&sig_mask) != 0) {
    err_quit("sigemptyset error");
  }
  sa.sa_handler = sig_handler;
  sa.sa_mask = sig_mask;
  sa.sa_flags = 0;
  new_time.it_interval.tv_sec = 5;  /*设置定时器每间隔5秒执行一次 */
  new_time.it_interval.tv_usec = 0;
  /* it_value 不能为0 为0 定时器不执行*/
  new_time.it_value.tv_sec = 5;  /* 程序启动时第一次调用的时间 */
  new_time.it_value.tv_usec = 0;
  /* 设置信号处理函数 */
  if (sigaction(SIGALRM,&sa,NULL) != 0) {
    err_quit("sigaction error");
  }
  /* 创建一个定时器 */
  if (setitimer(ITIMER_REAL,&new_time,NULL) != 0) {
    err_quit("setitimer error");
  }
  while(1);
  return 0;
}
