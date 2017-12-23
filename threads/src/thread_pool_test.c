/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:26
 * 线程池测试代码
 */
#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"

void *
start_routine1(void *arg) {
    printf("1\n");
    return arg;
}

void *
start_routine2(void *arg) {
    printf("2\n");
    return arg;
}

void *
start_routine3(void *arg) {
    printf("3\n");
    return arg;
}

int
main(int argc, const char *argv[])
{
    int i;
    thread_pool *pool = thread_pool_init(4);
    for(i = 0; i < 100; i++) {
        thread_pool_add_work(pool,start_routine1,NULL);
        thread_pool_add_work(pool,start_routine2,NULL);
        thread_pool_add_work(pool,start_routine3,NULL);
    }
    thread_pool_destroy(pool);
    return 0;
}