/*
 * (C) XuYuanChang 2017
 * 2017/12/20 10:08
 * 互斥量
 */

#include "main.h"
#include <pthread.h>

static long int global = 0;

pthread_mutex_t mutex;

void
increment(int loop) {
    int i = 0;
    for (i = 0; i < loop; i++) {
        pthread_mutex_lock(&mutex);
        global++;
        pthread_mutex_unlock(&mutex);
    }
}

void *
start_routine(void *arg) {
    int loop = (int)arg;
    increment(loop);
    return NULL;
}

int
main(int argc, const char *argv[])
{
    if (argc != 2) {
        err_msg("Usage pthread_mutex1 [threads number]");
        exit(0);
    }
    int i;
    long long int size = atoll(argv[1]);
    void *retval;
    pthread_t threads [size];
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
        err_sys("pthread_attr_init error");
    /* 设置线程栈的大小 */
    if (pthread_attr_setstacksize(&attr,102400) != 0)
        err_sys("pthread_attr_setstacksize error");
    if (pthread_mutex_init(&mutex,NULL) != 0)
        err_sys("pthread_mutex_init error");
    /* 创建线程 */
    for (i = 0; i < size; ++i) {
        if (pthread_create(&threads[i],&attr,start_routine,(void *)1000) != 0)
            err_sys("pthread_create error");
    }
    /* 回收线程 */
    for (i = 0; i < size; ++i) {
        if (pthread_join(threads[i],&retval) != 0)
            continue;
    }
    printf("global = %ld\n",global);
    return 0;
}