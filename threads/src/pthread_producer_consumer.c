/*
 * (C) XuYuanChang 2017
 * 2017/12/20 17:23
 * 生产者消费者模型
 */

#include "main.h"
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int alive = 0;

void *
producer_routine(void *arg)
{
    for (;;) {
        pthread_mutex_lock(&mutex);
        alive++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

void *
consumer_routine(void *arg)
{
    for(;;) {
        pthread_mutex_lock(&mutex);
        while (alive == 0) {
            if (pthread_cond_wait(&cond,&mutex) != 0)
                err_sys("pthread_cond_wait error");
        }
        while (alive > 0) {
            printf("consumer alive = %d\n",alive);
            alive--;
        }
        printf("consumer alive = %d\n",alive);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int
main(int argc, const char *argv[])
{
    if (argc != 2) {
        err_sys("Usage pthread_producer_consumer [consumer thread numbers]");
    }
    int i;
    void *retval;
    int numbers = atoi(argv[1]);
    pthread_t producer;
    pthread_t consumers[numbers];
    /* 创建生产者线程 */
    if (pthread_create(&producer,NULL,producer_routine,NULL) != 0)
        err_sys("pthread_create error");
    /* 创建消费者线程 */
    for (i = 0; i < numbers; ++i) {
        if(pthread_create(&consumers[i],NULL,consumer_routine,NULL) != 0)
            err_sys("pthread_create error");
    }
    /* 回收线程 */
    for (i = 0; i < numbers; i++) {
        if (pthread_join(consumers[i],&retval) !=0 )
            err_sys("pthread_join error");
    }
    if (pthread_join(producer,&retval) !=0 )
        err_sys("pthread_join error");
    return 0;
}