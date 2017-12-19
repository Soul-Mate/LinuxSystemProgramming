/*
 * (C) XuYuanChang 2017
 * 创建多个线程代码示例
 */
#include "main.h"
#include <pthread.h>

void *start_routine(void *);


void
*start_routine(void *arg) {
    printf("0x%lu: 第 %s 个线程\n",pthread_self(),(char *)arg);
    return NULL;
}

int
main(int argc, const char *argv[]) {
    pthread_t thread;
    if (argc != 2) {
        printf("Usage pthread_create2 [thread number]\n");
        exit(0);
    }
    int i,size;
    size = atoi(argv[1]);
    pthread_t threads[size];
    for (i = 0; i < size; ++i) {
        char *str = malloc(25);
        sprintf(str,"%d",i);
        if (pthread_create(&threads[i],NULL,start_routine,(void *)str) != 0)
            err_msg("pthread_create error");
    }
    sleep(1);
}
