/*
 * (C) XuYuanChang 2017
 * 线程退出
 */
#include "main.h"
#include <pthread.h>

void *start_routine(void *);

void
*start_routine(void *arg) {
    int num = atoi((char *)arg);
    /* 小于3的线程退出 */
    if (num < 3) {
        printf("%d 线程退出\n",num);
        pthread_exit(NULL);
    }
    pthread_t tid = pthread_self();
    printf("0x%lu: 第 %s 个线程\n",(long)tid,(char *)arg);
    sleep(num);
    return arg;
}

int
main(int argc,const char *argv[]) {
    pthread_t thread;
    if (argc != 2) {
        printf("Usage pthread_exit [thread number]\n");
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
        if (ret_val == NULL) {
            printf("回收退出线程\n");
        }else {
            printf("%s 线程被回收\n",(char *)ret_val);
        }
    }
}
