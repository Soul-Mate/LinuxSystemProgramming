/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:26
 * 线程池测试代码
 */
#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"

void *
start_routine(void *arg) {
    printf("hello world!\n");
    return arg;
}

int
main(int argc, const char *argv[])
{
    int i;
    thread_pool *pool = thread_pool_init(4);
    for(i = 0; i < 10; i++) {
        thread_pool_add_work(pool,start_routine,NULL);
    }
    thread_pool_wait(pool);
    return 0;
}