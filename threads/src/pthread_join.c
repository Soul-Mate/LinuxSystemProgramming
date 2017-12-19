/*
 * (C) XuYuanChang 2017
 * 连接多个线程
 */
#include "main.h"
#include <pthread.h>

void *start_routine(void *);


void
*start_routine(void *arg) {
    pthread_t tid = pthread_self();
    printf("0x%lu: 第 %s 个线程\n",(long)tid,(char *)arg);
    sleep(atoi((char *)arg));
    return arg;
}

int
main(int argc,const char *argv[]) {
    pthread_t thread;
    if (argc != 2) {
        printf("Usage pthread_join [thread number]\n");
        exit(0);
    }
    void *ret_val;
    int i,size;
    size = atoi(argv[1]);
    pthread_t threads[size];
    for (i = 0; i < size; ++i) {
        char *str = malloc(25);
        sprintf(str,"%d",i);
        if (pthread_create(&threads[i],NULL,start_routine,(void *)str) != 0)
            err_msg("pthread_create error");
    }
    int len = sizeof(threads) / sizeof(pthread_t);
    for (i = 0; i < len; ++i) {
        if (pthread_join(threads[i],&ret_val) != 0)
            err_msg("pthread_join error");
        printf("%s 线程被回收\n",(char *)ret_val);
    }
}
