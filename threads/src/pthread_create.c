/*
 * (C) XuYuanChang 2017
 * 创建线程示例
 */
#include "main.h"
#include <pthread.h>

void *start_routine(void *);

void
*start_routine(void *arg) {
    char *s;
    s = malloc(strlen(arg));
    s = (char *)arg;
    printf("0x%lu:\n\t%s",pthread_self(),s);
    return (void *)NULL;
}

int
main(int argc, const char *string) {
    pthread_t thread;
    if (pthread_create(&thread,NULL,start_routine,"hello world!\n") != 0)
        err_msg("pthread_create error");
    sleep(1);
}


