/*
 * (C) XuYuanChang 2017
 * 2017/12/21 10:26
 * 线程池测试代码
 */
#include "threadpool.h"

void *
start_routine(void *arg) {
    printf("hello world!\n");
    return arg;
}

int
main(int argc, const char *argv[])
{
    thread_job *job = (thread_job *)malloc(sizeof(thread_job));
    job->next = NULL;
    job->arg = NULL;
    job->func = start_routine;
    thread_pool * pool = thread_pool_init(4);
    thread_job_queue_push(pool->job_queue,job);
    thread_job_queue_push(pool->job_queue,job);
    thread_job_queue_push(pool->job_queue,job);
    thread_job_queue_push(pool->job_queue,job);
    thread_job_queue_push(pool->job_queue,job);
    thread_job_queue_push(pool->job_queue,job);
    while (pool->job_queue->len) {
        thread_job *test_job = thread_job_pull(pool->job_queue);
        test_job->func(test_job->arg);
    }
}