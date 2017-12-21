/*
 * (C) XuYuanChang 2017
 * since 2017/12/20
 * 线程设置分离态
 */


#include "main.h"
#include <pthread.h>

void *
start_routine(void *arg)
{
    printf("%s\n","start_routine");
    return NULL;
}

int
main(int argc, const char *argv[])
{
    pthread_t thread;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        err_sys("pthread_attr_init error");
    int detachstate = PTHREAD_CREATE_DETACHED;
    if (pthread_attr_setdetachstate(&attr,detachstate) != 0)
        err_sys("pthread_attr_setdetachstate error");
    if (pthread_create(&thread,&attr,start_routine,NULL) != 0)
        err_sys("pthread_create error");
}