/*
对阻塞的系统调用设置设置超时时间
1. 设置sigaction捕捉SIGALRM信号,同时需要清楚SA_RESTART标志防止系统调用重新启用
2. 设置定时器
3. 执行会发生阻塞的系统调用
4. 处理调用在定时器到时之前完成的情况：系统调用返回后，屏蔽定时器
5. 检查系统调用是否是被中断的 即errno 是否等于EINTR
*/

#include "main.h"
#include <time.h>
#include <sys/time.h>

void sig_handler(int signo)
{
  printf("timeout\n");
  return;
}

int main(int argc, char const *argv[])
{
  if (argc < 2) {
    err_quit("Usage alarm_read [timeout]");
  }
  sigset_t sig_mask;
  sigemptyset(&sig_mask);
  char buf[1024];
  int errno_saved,read_num;
  struct itimerval new_value;
  struct sigaction sa;
  new_value.it_value.tv_sec = atoi(argv[1]);
  new_value.it_value.tv_usec = 0;
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_usec = 0;

  sa.sa_handler = sig_handler;
  sa.sa_mask = sig_mask;
  sa.sa_flags = 0;
  /* 设置捕获SIGALRM */
  sigaction(SIGALRM,&sa,NULL);
  /* 设置定时器 */
  if (setitimer(ITIMER_REAL,&new_value,NULL) != 0) {
    err_quit("setitimer error");
  }
  /* 执行会发生阻塞的系统调用 */
  read_num = read(STDIN_FILENO,&buf,1024);
  errno_saved = errno;
  /* 如果系统调用已经完成，屏蔽定时器 */
  if (setitimer(ITIMER_REAL,NULL,NULL) != 0) {
    err_quit("setitimer error");
  }
  errno = errno_saved;
  /* 判定系统调用是否被中断 */
  if (read_num < 0) {
    if (errno == EINTR) {
      err_quit("read timeout");
    } else {
      err_quit("read error");
    }
  }else {
    printf("read success\n");
  }
  return 0;
}
