/*
 * (C) XuYuanChang 2017
 * 设置和获取线程栈大小
 */

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif

#include "main.h"
#include <pthread.h>

void get_stacksize(const pthread_attr_t *restrict attr) {
    size_t size;
    /*获取当前线程栈大小*/
    if (pthread_attr_getstacksize(attr,&size) != 0)
        err_quit("pthread_attr_getstacksize error");
    printf("current thread stack size: %lu\n",size);
}

int
main(int argc,const char *argv[])
{
    if (argc !=2 ) {
        printf("Usage pthread_stacksize [size]\n");
        exit(0);
    }
    int size = atoi(argv[1]);
    if (size < 0 || size < PTHREAD_STACK_MIN)
        err_quit("size error");
    size_t set_size;
    set_size = size;
    pthread_attr_t attr;
    /* 初始化线程属性*/
    if (pthread_attr_init(&attr) != 0)
        err_quit("pthread_attr_init error");
    get_stacksize(&attr);
    /*设置线程栈大小*/
    if (pthread_attr_setstacksize(&attr,set_size) != 0)
        err_quit("pthread_attr_setstacksize error");
    get_stacksize(&attr);
}