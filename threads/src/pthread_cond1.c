/*
 * (C) XuYuanChang 2017
 * 2017/12/20 13:40
 * 条件变量
 */

#include "main.h"
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static long int global;

void *
increment_routine(void *arg)
{
    int i,loop;
    loop = 100000;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < loop; ++i) {
        global++;
    }
    pthread_mutex_unlock(&mutex);
    /* 通知当前阻塞的线程 唤醒变量 */
    pthread_cond_broadcast(&cond);
    return NULL;
}

void *
start_routine(void *arg)
{
    pthread_mutex_lock(&mutex);
    while(global < 10000) {
        printf("%ld\n",global);
        pthread_cond_wait(&cond,&mutex);
    }
    printf("global > 10000, global = %ld\n",global);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int
main(int argc,const char *argv[])
{
    if (argc != 2)
        err_sys("Usage pthread_cond1 [thread numbers]");
    int i;
    void *retval;
    int numbers = atoi(argv[1]);
    pthread_t thread,threads[numbers];
    /* 创建线程 用于输出 当global > 10000 */
    for (i = 0; i < numbers; ++i) {
        if (pthread_create(&threads[i],NULL,start_routine,NULL) !=0 )
            err_sys("pthread_create error");
    }
    /* 创建线程 对global执行++操作 */
    if (pthread_create(&thread,NULL,increment_routine,NULL) !=0 )
        err_sys("pthread_create error");
    /* 回收线程 */
    for (i = 0; i < numbers; ++i) {
        if (pthread_join(threads[i],&retval) != 0)
            err_msg("pthread_join error");
    }
    if (pthread_join(thread,&retval) != 0)
        err_msg("pthread_join error");
    return 0;
}