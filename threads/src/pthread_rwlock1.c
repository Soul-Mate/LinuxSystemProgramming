/*
 * (C) XuYuanChang 2017
 * 2017/12/20 10:58
 * 读写锁
 */

#include "main.h"
#include <math.h>
#include <pthread.h>

typedef struct {
    int loop;
    char type;
}thread_struct;

static long int global;

pthread_rwlock_t rwlock;

void
increment(int loop)
{
    int i = 0;
    for (i = 0; i < loop; i++) {
        pthread_rwlock_wrlock(&rwlock);
        global++;
        pthread_rwlock_unlock(&rwlock);
    }
    sleep(1);
}

long int
getglobal()
{
    return global;
}

void *
start_routine(void *arg)
{
    thread_struct *ts = malloc(sizeof(thread_struct));
    ts = (thread_struct *)arg;
    switch (ts->type) {
        case 'r':
            printf("%ld\n",getglobal());
            break;
        case 'w':
            increment(ts->loop);
            break;
    }
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        err_msg("Usage pthread_rwlock1 [threads number]\n");
        exit(0);
    }
    int i;
    int size = atoi(argv[1]) / 2;
    void *retval;
    pthread_t threads_r[size];
    pthread_t threads_w[size];
    thread_struct ts1,ts2;
    ts1.loop = 1000;
    ts1.type = 'w';
    ts2.loop = 1000;
    ts2.type = 'r';
    /* 初始化读写锁 */
    if (pthread_rwlock_init(&rwlock,NULL) != 0)
        err_sys("pthread_rwlock_init error");
    /* 创建线程进行写 */
    for (i = 0; i < size; ++i) {
        if (pthread_create(&threads_w[i],NULL,start_routine,(void *)&ts1) != 0)
            err_sys("pthread_create error");
    }
    /* 创建线程进行读 */
    for (i = 0; i < size; ++i) {
        if (pthread_create(&threads_r[i],NULL,start_routine,(void *)&ts2) != 0)
            err_sys("pthread_create error");
    }
     /* 回收线程 */
    for (i = 0; i < size; ++i) {
        pthread_join(threads_r[i],&retval);
        pthread_join(threads_w[i],&retval);
    }
    if (pthread_rwlock_destroy(&rwlock) != 0)
        err_sys("pthread_rwlock_destroy error");
    return 0;
}